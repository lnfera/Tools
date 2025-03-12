#pragma once

struct ModelVertexToPixel
{
    float4 position : SV_POSITION;
    float4 worldPosition : POSITION;
    float4 vertexColor0 : COLOR0;
    float4 vertexColor1 : COLOR1;
    float4 vertexColor2 : COLOR2;
    float4 vertexColor3 : COLOR3;
    float2 texCoord0 : TEXCOORD0;
    float2 texCoord1 : TEXCOORD1;
    float2 texCoord2 : TEXCOORD2;
    float2 texCoord3 : TEXCOORD3;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
    float depth : DEPTH;
};

struct GBufferOutput
{
	//float4 myWorldPosition : SV_TARGET0;
    float4 myAlbedo : SV_TARGET0;
    float4 myNormal : SV_TARGET1;
    float4 myMaterial : SV_TARGET2;
    float4 myAmbientOcclusionAndCustom : SV_TARGET3;
    float4 myWorldNormal : SV_TARGET4;
    float4 fragment : SV_TARGET5;
};

Texture2D Texture1 : register(t1); //albedoTexture
Texture2D Texture2 : register(t2); //normalTexture
Texture2D Texture3 : register(t3); //materialTexture
Texture2D Texture4 : register(t4); //fxTexture

Texture2D DeferedPosition : register(t6); // DepthShader

SamplerState Sampler : register(s0); // defaultSampler

cbuffer FrameBuffer : register(b0)
{
    uint2 Resolution;
    float DeltaTime;
    float TotalTime;
}

cbuffer CameraBuffer : register(b2)
{
    float4x4 toCamera; // Camera to Projection 
    float4x4 toProjection; //Projection Matrix  
    float4 cameraPosition;
    float NearP;
    float FarP;
    float2 padding;
}

cbuffer ObjectBuffer : register(b4)
{
    float4 UVOffset;
    float4x4 ObjectToWorld;
    float4x4 InverseObjectToWorld;
}

struct PixelOutput
{
    float4 color : SV_TARGET;
};

float2 isInsideAABB(float3 aPoint /* World Position Behind */, float4x4 aBoxTransform /*Box Collider*/)
{
    
    float3 boxPos = float3(aBoxTransform._14, aBoxTransform._24, aBoxTransform._34);
    
    boxPos.y += length(aBoxTransform[2].xyz) * 0.5f; // move pivot
    
    float boxSize = 25.f;
    
    float sizeX = boxSize * length(aBoxTransform[0].xyz);
    float sizeY = boxSize * length(aBoxTransform[1].xyz);
    float sizeZ = boxSize * length(aBoxTransform[2].xyz);
    
    if (aPoint.x < boxPos.x + sizeX && aPoint.x > boxPos.x - sizeX)
    {
        if (aPoint.z < boxPos.z + sizeZ && aPoint.z > boxPos.z - sizeZ)
        {
            if (aPoint.y < boxPos.y + sizeY && aPoint.y > boxPos.y - sizeY)
            {
                
                // Textcoords within Cube
                float xval = (aPoint.x - boxPos.x - sizeX) / (sizeX * 2);
                float zval = (aPoint.z - boxPos.z - sizeZ) / (sizeZ * 2);
                
                return float2(xval,zval);
            }
        }
    }
    return float2(-1,-1);
}

float2 isInsideOOBB(float3 aPoint, float4x4 aBoxTransform)
{
    float3 boxPos = float3(aBoxTransform._14, aBoxTransform._24, aBoxTransform._34);
    
    // Extract the box orientation and scale
    float3 right = normalize(float3(aBoxTransform._11, aBoxTransform._21, aBoxTransform._31)); // X-axis
    float3 up =     normalize(float3(aBoxTransform._12, aBoxTransform._22, aBoxTransform._32)); // Y-axis
    float3 forward = normalize(float3(aBoxTransform._13, aBoxTransform._23, aBoxTransform._33)); // Z-axis

    float boxSize = 45.f;

    float sizeX = boxSize * length(float3(aBoxTransform._11, aBoxTransform._21, aBoxTransform._31));
    float sizeY = boxSize * length(float3(aBoxTransform._12, aBoxTransform._22, aBoxTransform._32));
    float sizeZ = boxSize * length(float3(aBoxTransform._13, aBoxTransform._23, aBoxTransform._33));

    // Move pivot to center
    boxPos += up * (sizeY * 0.5f);

    // Convert point into OBB local space
    float3 localPos = aPoint - boxPos;
    float3 proj = float3(dot(localPos, right), dot(localPos, up), dot(localPos, forward));

    // AABB Check in Local Space
    if (abs(proj.x) <= sizeX && abs(proj.y) <= sizeY && abs(proj.z) <= sizeZ)
    {
        // Convert to normalized texture coordinates (0 to 1)
        float xval = (proj.x + sizeX) / (sizeX * 2);
        float zval = (proj.z + sizeZ) / (sizeZ * 2);
        
        return float2(xval + (TotalTime * UVOffset.x), zval + (TotalTime * UVOffset.y));
    }

    return float2(-1, -1);
}

GBufferOutput main(ModelVertexToPixel input)
{
    GBufferOutput output;

    float2 uv = input.position.xy / Resolution.xy;
    float4 worldPosition = DeferedPosition.Sample(Sampler, uv).rgba;
    
    float2 textCoords = isInsideOOBB(worldPosition.xyz, ObjectToWorld);
    
    if (textCoords.x != -1 || textCoords.y != -1 )
    {
        output.myAlbedo = Texture1.Sample(Sampler, textCoords);
        output.myNormal = Texture2.Sample(Sampler, textCoords);
        output.myMaterial = Texture3.Sample(Sampler, textCoords);
        output.myAmbientOcclusionAndCustom = Texture4.Sample(Sampler, textCoords);
        
        output.myWorldNormal = 0;
        output.fragment = 0;

    }
    else
    {
        output.myAlbedo = 0;
        output.myNormal = 0;
        output.myMaterial = 0;
        output.myAlbedo = 0;
        output.myAlbedo = 0;
        output.myWorldNormal = 0;
        output.fragment = 0;
        discard;
    }

    return output;
    
}


//PixelOutput output;
    
    //float2 uv = input.position.xy / Resolution.xy;
    //float4 worldPosition = DeferedPosition.Sample(Sampler, uv).rgba;
    //float depth = DepthBuffer.Sample(Sampler, uv).rgba;
    
    //float4 ndcPos = mul(inverse(ObjectToWorld), worldPosition);
    //ndcPos.xyz /= ndcPos.w;
    //ndcPos.xy *= (Resolution.x / Resolution.y);
    
    //if (ndcPos.x < -1.0 || ndcPos.x > 1.0 || ndcPos.y < -1.0 || ndcPos.y > 1.0)
    //    discard;
    
    
    //float2 decalTexCoord = ndcPos.xy * 0.5 + 0.5;
    //decalTexCoord.x = 1.0 - decalTexCoord.x;
    
    //float4 albedo = Texture1.Sample(Sampler, decalTexCoord);
    //output.color = albedo;
 
    //return output;