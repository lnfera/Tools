#pragma once
#include "../common.hlsli"
struct GBufferOutput
{
    float4 myWorldPosition  : SV_TARGET0;
    float4 myAlbedo         : SV_TARGET1;
    float4 myNormal         : SV_TARGET2;
    float4 myMaterial       : SV_TARGET3;
    float4 myAmbientOcclusionAndCustom : SV_TARGET4;
};

GBufferOutput main(ModelVertexToPixel pixel)
{
    float2 scaledUV = pixel.texCoord0;
    
    float4 albedo = albedoTexture.Sample(defaultSampler, scaledUV).rgba;
    float3 normal = normalTexture.Sample(defaultSampler, scaledUV).xyz;
    float4 material = materialTexture.Sample(defaultSampler, scaledUV);
    float4 fx = fxTexture.Sample(defaultSampler, scaledUV);
    
    float ambientOcclusion = normal.b;
    
    normal.xy = 2.0f * normal.xy - 1.0f;
    normal.z = sqrt(1 - saturate(normal.x * normal.x + normal.y * normal.y));
    normal = normalize(normal);
    
    float3x3 TBN = float3x3(
        normalize(pixel.tangent.xyz),
        normalize(-pixel.binormal.xyz),
        normalize(pixel.normal.xyz)
        );
    TBN = transpose(TBN);
    
    float3 pixelNormal = normalize(mul(TBN, normal));
   
    GBufferOutput output = (GBufferOutput)0;
    output.myWorldPosition = pixel.worldPosition;
    output.myAlbedo = albedo;
    output.myNormal = float4(0.5f + pixelNormal * 0.5f, 1.0f);
    output.myMaterial = material;
    output.myAmbientOcclusionAndCustom = float4(ambientOcclusion, /*Emission*/fx.r, 1.f, 1.f); // gba are unused, put whatever data you want here!
    
    if (albedo.w <= alphaTestThreshold)
    {
        discard;
        output.myAlbedo = float4(0.0f, 0.0f, 0.0f, 0.0f);
        return output;
    }
    
    return output;
}