#pragma once
#include <string>

namespace SHADER
{

	//const std::string CommonHLSLI = R"(
	//
	//)";
#pragma region Common

	const std::string BuffersHLSLI = R"(
cbuffer FrameBuffer : register(b0)
{
	float4 Resolution; //myResolution.x = screen width, myResolution.y = screen height, myResolution.z = unset, myResolution.w = unset
	float4 Timings; //myTimings.x = totaltime, myTimings.y = delta time, myTimings.z = unset, myTimings.w = unset
}

cbuffer CameraBuffer : register(b1)
{
	float4x4 WorldToCamera;
	float4x4 CameraToProjection;
	float4 CameraPosition;
	float NearPlane;
	float FarPlane;
	float Unused0;
	float Unused1;
};

cbuffer LightConstantBufferData : register(b2)
{
	struct PointLightData
	{
		float4 Position;
		float4 Color;
		float Range;
		float3 garbage; // Padding, don't use arrays!
	} myPointLights[8];

	uint NumberOfLights;
	int NumEnvMapMipLevels;
	float garbage0;
	float garbage1;

	float4 AmbientLightColorAndIntensity;
	float4 DirectionalLightDirection;
	float4 DirectionalLightColorAndIntensity;
};

cbuffer ShaderSettingsConstantBuffer : register(b3)
{
	float4 customShaderParameters;
	float alphaTestThreshold;
};

cbuffer ObjectBuffer : register(b4)
{
	float4x4 ObjectToWorld;
}

cbuffer CustomShapeConstantBufferData : register(b4)
{
	float4x4 ModelToWorld;
};

cbuffer BoneBuffer : register(b5)
{
	float4x4 Bones[64];
};

)";

	const std::string ImagesAndSampler = R"(
	SamplerState defaultSampler : register(s0);

    TextureCube environmentTexture : register(t0);
    Texture2D imageText1z	: register(t1); // Albedo
    Texture2D imageText2z	: register(t2); // Normal
    Texture2D imageText3z	: register(t3); // Material
    Texture2D imageText4z	: register(t4); // FX + Extra
    Texture2D imageText5z	: register(t5);
    Texture2D imageText6z	: register(t6);
    Texture2D imageText7z	: register(t7);
    Texture2D imageText8z	: register(t8);
    Texture2D imageText9z	: register(t9);
    Texture2D imageText10z	: register(t10);
    Texture2D imageText11z	: register(t11);
    Texture2D imageText12z	: register(t12);
    Texture2D imageText13z	: register(t13);
    Texture2D imageText14z	: register(t14);
    Texture2D imageText15z	: register(t15);


)";

#pragma endregion

#pragma region Structs
	const std::string DeferredStructHLSLI = R"(

struct GBufferOutput
{
    float4 myWorldPosition				: SV_TARGET0;
    float4 myAlbedo						: SV_TARGET1;
    float4 myNormal						: SV_TARGET2;
    float4 myMaterial					: SV_TARGET3;
    float4 myAmbientOcclusionAndCustom	: SV_TARGET4;
};
)";


	const std::string StructsHLSLI = R"(
struct SpriteVertexToPixel
{
	float4 position			:	SV_POSITION;
	float4 worldPosition	:	POSITION;
	float2 tex				:	TEXCOORD0;
	float4 color			:	TEXCOORD2;
	float3 normal			:	NORMAL;
};
struct ModelVertexInput
{
	float4 position	    :	POSITION;
	float4 vertexColor0	:	COLOR0;
	float4 vertexColor1	:	COLOR1;
	float4 vertexColor2	:	COLOR2;
	float4 vertexColor3	:	COLOR3;
	float2 texCoord0	:	TEXCOORD0;
	float2 texCoord1	:	TEXCOORD1;
	float2 texCoord2	:	TEXCOORD2;
	float2 texCoord3	:	TEXCOORD3;
	float3 normal		:	NORMAL;
	float3 tangent		:	TANGENT;
	float3 binormal	    :	BINORMAL;
	float4 boneIndices  :   BONES;
	float4 weights      :   WEIGHTS;
};

struct ModelVertexToPixel
{
	float4 position			:	SV_POSITION;
	float4 worldPosition	:	POSITION;
	float4 vertexColor0		:	COLOR0;
	float4 vertexColor1		:	COLOR1;
	float4 vertexColor2		:	COLOR2;
	float4 vertexColor3		:	COLOR3;
	float2 texCoord0		:	TEXCOORD0;
	float2 texCoord1		:	TEXCOORD1;
	float2 texCoord2		:	TEXCOORD2;
	float2 texCoord3		:	TEXCOORD3;
	float3 normal			:	NORMAL;
	float3 tangent			:	TANGENT;
	float3 binormal			:	BINORMAL;
	float  depth		    :	DEPTH;
};

struct InstancedPixelInputType
{
	float4 position			:	SV_POSITION;
	float4 worldPosition	:	POSITION;
	float2 tex				:	TEXCOORD0;
	float4 color			:	TEXCOORD2;
	float3 normal			:	NORMAL;
	uint instanceId			:	SV_InstanceID;
};

struct PixelOutput
{
	float4 color		:	SV_TARGET;
};
)";
	//const std::string PixelHLSLI = R"(
	//
	//)";
#pragma endregion

#pragma region Functions

	const std::string Functions = R"(
	
int GetNumMips(TextureCube cubeTex)
{
	int iWidth = 0;
	int iheight = 0;
	int numMips = 0;
	cubeTex.GetDimensions(0, iWidth, iheight, numMips);
	return numMips;
}

float bias(float value, float b)
{
    return (b > 0.0) ? pow(abs(value), log(b) / log(0.5)) : 0.0f;
}

float gain(float value, float g)
{
    return 0.5 * ((value < 0.5) ? bias(2.0 * value, 1.0 - g) : (2.0 - bias(2.0 - 2.0 * value, 1.0 - g)));
}

float RoughnessFromPerceptualRoughness(float perceptualRoughness)
{
    return perceptualRoughness * perceptualRoughness;
}

float PerceptualRougnessFromRoughness(float roughness)
{
    return sqrt(max(0.0, roughness));
}

float SpecularPowerFromPerceptualRoughness(float perceptualRoughness)
{
    float f_epsilon = 1.192092896e-07f;
    float roughness = RoughnessFromPerceptualRoughness(perceptualRoughness);
    return (2.0 / max(f_epsilon, roughness * roughness)) - 2.0;
}

float PerceptualRougnessFromSpecularPower(float specularPower)
{
    float roughness = sqrt(2.0 / (specularPower + 2.0));
    return PerceptualRougnessFromRoughness(roughness);
}

float BurleyToMip(float fPerceptualRoughness, int nMips, float NdotR)
{
     int nMipOffset  = 3;

    float f_epsilon = 1.192092896e-07f;

    float specPower = SpecularPowerFromPerceptualRoughness(fPerceptualRoughness);
    specPower /= (4 * max(NdotR, f_epsilon));
    float scale = PerceptualRougnessFromSpecularPower(specPower);
    return scale * (nMips - 1 - nMipOffset);
}

float3 GetSpecularDominantDir(float3 vN, float3 vR, float roughness)
{
    float invRough = saturate(1 - roughness);
    float alpha = invRough * (sqrt(invRough) + roughness);

    return lerp(vN, vR, alpha);
}

float GetReductionInMicrofacets(float perceptualRoughness)
{
    float roughness = RoughnessFromPerceptualRoughness(perceptualRoughness);

    return 1.0 / (roughness * roughness + 1.0);
}

float EmpiricalSpecularAO(float ao, float perceptualRoughness)
{
    float smooth = 1 - perceptualRoughness;
    float specAO = gain(ao, 0.5 + max(0.0, smooth * 0.4));

    return min(1.0, specAO + lerp(0.0, 0.5, smooth * smooth * smooth * smooth));
}

float ApproximateSpecularSelfOcclusion(float3 vR, float3 vertNormalNormalized)
{
    const float fadeParam = 1.3;
    float rimmask = clamp(1 + fadeParam * dot(vR, vertNormalNormalized), 0.0, 1.0);
    rimmask *= rimmask;

    return rimmask;
}

float3 Diffuse(float3 pAlbedo)
{
    float PI = 3.14159265358979323846f;
    return pAlbedo / PI;
}

float NormalDistribution_GGX(float a, float NdH)
{
    float PI = 3.14159265358979323846f;

    // Isotropic ggx
    float a2 = a * a;
    float NdH2 = NdH * NdH;

    float denominator = NdH2 * (a2 - 1.0f) + 1.0f;
    denominator *= denominator;
    denominator *= PI;

    return a2 / denominator;
}

float Geometric_Smith_Schlick_GGX(float a, float NdV, float NdL)
{
    // Smith Schlick-GGX
    float k = a * 0.5f;
    float GV = NdV / (NdV * (1 - k) + k);
    float GL = NdL / (NdL * (1 - k) + k);

    return GV * GL;
}

float3 Fresnel_Schlick(float3 specularColor, float3 h, float3 v)
{
    return (specularColor + (1.0f - specularColor) * pow((1.0f - saturate(dot(v, h))), 5));
}

float3 Specular(float3 specularColor, float3 h, float3 v, float a, float NdL, float NdV, float NdH)
{
    return ((NormalDistribution_GGX(a, NdH) * Geometric_Smith_Schlick_GGX(a, NdV, NdL)) * Fresnel_Schlick(specularColor, h, v)) / (4.0f * NdL * NdV + 0.0001f);
}

float3 EvaluateAmbiance(TextureCube lysBurleyCube, float3 vN, float3 VNUnit, float3 toEye, float perceptualRoughness, float ao, float3 dfcol, float3 spccol)
{
     int nMipOffset  = 3;
    int numMips = GetNumMips(lysBurleyCube);
    const int nrBrdMips = numMips - nMipOffset;
    float VdotN = saturate(dot(toEye, vN));//clamp(dot(toEye, vN), 0.0, 1.0f);
    const float3 vRorg = 2 * vN * VdotN - toEye;

    float3 vR = GetSpecularDominantDir(vN, vRorg, RoughnessFromPerceptualRoughness(perceptualRoughness));
    float RdotNsat = saturate(dot(vN, vR));

    float mipLevel = BurleyToMip(perceptualRoughness, numMips, RdotNsat);

    float3 specRad = lysBurleyCube.SampleLevel(defaultSampler, vR, mipLevel).xyz;
    float3 diffRad = lysBurleyCube.SampleLevel(defaultSampler, vN, (float)(nrBrdMips - 1)).xyz;

    float fT = 1.0 - RdotNsat;
    float fT5 = fT * fT;
    fT5 = fT5 * fT5 * fT;
    spccol = lerp(spccol, (float3) 1.0, fT5);

    float fFade = GetReductionInMicrofacets(perceptualRoughness);
    fFade *= EmpiricalSpecularAO(ao, perceptualRoughness);
    fFade *= ApproximateSpecularSelfOcclusion(vR, VNUnit);

    float3 ambientdiffuse = ao * dfcol * diffRad;
    float3 ambientspecular = fFade * spccol * specRad;

	return ambientdiffuse + ambientspecular;
}

float3 EvaluateDirectionalLight(float3 albedoColor, float3 specularColor, float3 normal, float roughness, float3 lightColor, float3 lightDir, float3 viewDir)
{
    // Compute som useful values
    float NdL = saturate(dot(normal, lightDir));
    float lambert = NdL; // Angle attenuation
    float NdV = saturate(dot(normal, viewDir));
    float3 h = normalize(lightDir + viewDir);
    float NdH = saturate(dot(normal, h));
    float VdH = saturate(dot(viewDir, h));
    float LdV = saturate(dot(lightDir, viewDir));
    float a = max(0.001f, roughness * roughness);

    float3 cDiff = Diffuse(albedoColor);
    float3 cSpec = Specular(specularColor, h, viewDir, a, NdL, NdV, NdH);
    float PI = 3.14159265358979323846f;

    return saturate(lightColor * lambert * (cDiff * (1.0 - cSpec) + cSpec) * PI);
}

float3 EvaluatePointLight(float3 albedoColor, float3 specularColor, float3 normal, float roughness,
    float3 lightColor, float lightIntensity, float lightRange, float3 lightPos, float3 viewDir, float3 pixelPos)
{
    // Compute som useful values
    float3 lightDir = lightPos.xyz - pixelPos.xyz;
    float lightDistance = length(lightDir);
    lightDir = normalize(lightDir);
	
    float NdL = saturate(dot(normal, lightDir));
    float lambert = NdL; // Angle attenuation
    float NdV = saturate(dot(normal, viewDir));
    float3 h = normalize(lightDir + viewDir);
    float NdH = saturate(dot(normal, h));
    float a = max(0.001f, roughness * roughness);

    float3 cDiff = Diffuse(albedoColor);
    float3 cSpec = Specular(specularColor, h, viewDir, a, NdL, NdV, NdH);

    float linearAttenuation = lightDistance / lightRange;
    linearAttenuation = 1.0f - linearAttenuation;
    linearAttenuation = saturate(linearAttenuation);
    float physicalAttenuation = saturate(1.0f / (lightDistance * lightDistance));
    float ue4Attenuation = ((pow(saturate(1 - pow(lightDistance / lightRange, 4.0f)), 2.0f)) / (pow(lightDistance, 2.0f) + 1)); // Unreal Engine 4 attenuation
    float attenuation = lambert * linearAttenuation * physicalAttenuation;
    attenuation = ue4Attenuation * lambert;
    float PI = 3.14159265358979323846f;

    return saturate(lightColor * lightIntensity * attenuation * ((cDiff * (1.0 - cSpec) + cSpec) * PI));
}

float3 EvaluateSpotLight(float3 albedoColor, float3 specularColor, float3 normal,
    float roughness, float3 lightColor, float lightIntensity, float lightRange,
    float3 lightPos, float3 lightDir, float outerAngle, float innerAngle, float3 viewDir, float3 pixelPos)
{
    float3 toLight = lightPos.xyz - pixelPos.xyz;
    float lightDistance = length(toLight);
    toLight = normalize(toLight);

    float NdL = saturate(dot(normal, toLight));
    float lambert = NdL; // Angle attenuation
    float NdV = saturate(dot(normal, viewDir));
    float3 h = normalize(toLight + viewDir);
    float NdH = saturate(dot(normal, h));
    float a = max(0.001f, roughness * roughness);

    float3 cDiff = Diffuse(albedoColor);
    float3 cSpec = Specular(specularColor, h, viewDir, a, NdL, NdV, NdH);

    float cosOuterAngle = cos(outerAngle);
    float cosInnerAngle = cos(innerAngle);
    float3 lightDirection = lightDir;

    // Determine if pixel is within cone.
    float theta = dot(toLight, normalize(-lightDirection));
	// And if we're in the inner or outer radius.
    float epsilon = cosInnerAngle - cosOuterAngle;
    float intensity = clamp((theta - cosOuterAngle) / epsilon, 0.0f, 1.0f);
    intensity *= intensity;
	
    float ue4Attenuation = ((pow(saturate(1 - pow(lightDistance / lightRange, 4.0f)), 2.0f)) / (pow(lightDistance, 2.0f) + 1)); // Unreal Engine 4 attenuation
	float finalAttenuation = lambert * intensity * ue4Attenuation;
    float PI = 3.14159265358979323846f;

    return saturate(lightColor * lightIntensity * lambert * finalAttenuation * ((cDiff * (1.0 - cSpec) + cSpec) * PI));
}

)";

#pragma endregion

#pragma region START_SHADER_END
	const std::string PixelStart = R"(
float4 main(ModelVertexToPixel input) : SV_TARGET
{
	float2 scaledUV = input.texCoord0;
	float3 toEye = normalize(CameraPosition.xyz - input.worldPosition.xyz);

	float4 output = float4(1,1,1,1);
	
)";

	const std::string START_DEFERRED = R"(
GBufferOutput main(ModelVertexToPixel input)
{
    float2 scaledUV = input.texCoord0;
	float3 toEye = normalize(CameraPosition.xyz - input.worldPosition.xyz);

	float4 albedo = float4(0,0,0,0);
	float4 normal = float4(0,0,0,0);
	float4 material = float4(0,0,0,0);
    float4 fx = float4(0,0,0,0);

	
	GBufferOutput output;

)";

	const std::string PBRStart = R"(

    PixelOutput main(ModelVertexToPixel input)
    {
	    PixelOutput result;
        float2 scaledUV = input.texCoord0;
	    float3 toEye = normalize(CameraPosition.xyz - input.worldPosition.xyz);

        float4 albedo   = float4(0,0,0,0);
	    float4 normal   = float4(0,0,0,0);
	    float4 material = float4(0,0,0,0);
        float4 fx       = float4(0,0,0,0);

        // Variables

)";

	const std::string PBREnd = R"(

        normal.xy = 2.0f * normal.xy - 1.0f;
	    normal.z = sqrt(1 - saturate(normal.x * normal.x + normal.y * normal.y));
	    normal = normalize(normal);

	    float3x3 TBN = float3x3(
	    	normalize(input.tangent.xyz),
	    	normalize(-input.binormal.xyz),
	    	normalize(input.normal.xyz)
	    	);

	    // Can save an instruction here by instead doing
	    // normalize(mul(normal, TBN)); It works because
	    // TBN is a 3x3 and therefore TBN^T is the same
	    // as TBN^-1. However, it is considered good form
	    // to do this.
	    TBN = transpose(TBN);
	    float3 pixelNormal = normalize(mul(TBN, normal));

	    // TGA Channel Pack. ORM.
	    // Metalness, Roughness, Emissive, Emissive Strength (opt).

        float ambientOcclusion = material.r;
        float metalness = material.b;
        float roughness = material.g;

        float emissive = fx.r;

	    float3 specularColor = lerp((float3) 0.04f, albedo.rgb, metalness);
	    float3 diffuseColor = lerp((float3) 0.00f, albedo.rgb, 1 - metalness);

	    float3 ambiance = EvaluateAmbiance(
	    	environmentTexture, pixelNormal, input.normal.xyz,
	    	toEye, roughness,
	    	ambientOcclusion, diffuseColor, specularColor
	    );

	    float3 directionalLight = EvaluateDirectionalLight(
	    	diffuseColor, specularColor, pixelNormal, roughness,
	    	DirectionalLightColorAndIntensity.xyz, DirectionalLightDirection.xyz, toEye.xyz
	    );

	    float3 pointLights = 0; // <- The sum of all point lights.
	    for(unsigned int p = 0; p < NumberOfLights; p++)
	    {
	    	pointLights += EvaluatePointLight(
	    		diffuseColor, specularColor, pixelNormal, roughness,
	    		myPointLights[p].Color.rgb, myPointLights[p].Color.w, myPointLights[p].Range, myPointLights[p].Position.xyz,
	    		toEye.xyz, input.worldPosition.xyz);
	    }
	    
	    float3 emissiveAlbedo = albedo.rgb * emissive;
	    float3 radiance = ambiance + directionalLight + pointLights + emissiveAlbedo;

        result.color.rgb = (float3) radiance;
	    result.color.a = albedo.a;
	    return result;
    }

)";

	const std::string ShaderEnd = R"(

	return output;
}

)";
	const std::string END_DEFERRED = R"(

	 float ambientOcclusion = normal.b;
    
    normal.xy = 2.0f * normal.xy - 1.0f;
    normal.z = sqrt(1 - saturate(normal.x * normal.x + normal.y * normal.y));
    normal = normalize(normal);
    
    float3x3 TBN = float3x3(
        normalize(input.tangent.xyz),
        normalize(-input.binormal.xyz),
        normalize(input.normal.xyz)
        );
    TBN = transpose(TBN);
    
    float3 pixelNormal = normalize(mul(TBN, normal));

	output.myWorldPosition = input.worldPosition;
    output.myAlbedo = albedo;
    output.myNormal = float4(0.5f + 0.5f * pixelNormal, 1.0f);
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
)";
#pragma endregion

} // end shader namespace
