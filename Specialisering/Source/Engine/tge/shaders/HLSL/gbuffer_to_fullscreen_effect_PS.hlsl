#pragma once
#include "../common.hlsli"
#include "../PBRFunctions.hlsli"
#include "../PostprocessStructs.hlsli"

Texture2D gBufferPositionTexture    : register(t1);
Texture2D gBufferAlbedoTexture      : register(t2);
Texture2D gBufferNormalTexture      : register(t3);
Texture2D gBufferMaterialTexture    : register(t4);
Texture2D gBufferAOAndCustom        : register(t5);


PostProcessPixelOutput main(PostProcessVertexToPixel input)
{
    PostProcessPixelOutput output;
    
    float2 uv = input.myPosition.xy / Resolution.xy;
    float4 worldPosition = gBufferPositionTexture.Sample(defaultSampler, uv).rgba;
    float4 albedo = gBufferAlbedoTexture.Sample(defaultSampler, uv).rgba;
    float3 normal = normalize(2.f * gBufferNormalTexture.Sample(defaultSampler, uv).xyz - 1.f);
    //float3 worldNormal = normalize(2.f * gBufferWorldNormal.Sample(defaultSampler, uv).xyz - 1.f);
    float4 material = gBufferMaterialTexture.Sample(defaultSampler, uv);
    float3 aoCustom = gBufferAOAndCustom.Sample(defaultSampler, uv);
    float3 toEye = normalize(CameraPosition.xyz - worldPosition.xyz);
    
    if (albedo.a <= alphaTestThreshold)
    {
        discard;
        output.myColor = float4(0.f, 0.f, 0.f, 0.f);
        return output;
    }
    
    float ambientOcclusion = material.r;
    float metalness = material.b;
    float roughness = material.g;
    
    //[Emissive]
    float emissive = aoCustom.g;
    
    float3 specularColor = lerp((float3) 0.04f, albedo.rgb, metalness);
    float3 diffuseColor = lerp((float3) 0.00f, albedo.rgb, 1 - metalness);

 //    //[Lights]
 //   float3 ambiance = EvaluateAmbiance(
 //   	environmentTexture, normal.xyz, worldNormal.xyz,
 //   	toEye, roughness,
 //   	ambientOcclusion, diffuseColor, specularColor
 //   );
    
    
 //   float3 directionalLight = EvaluateDirectionalLight(
	//	albedo.rgb, specularColor, worldNormal.xyz, roughness,
	//	DirectionalLightColorAndIntensity.xyz * DirectionalLightColorAndIntensity.w, DirectionalLightDirection.xyz, toEye.xyz
	//);
    
    //[Shadow]
    //float shadowFactor = 1.0f;
    
    //float4 directionalLightProjectedPositionTemp = mul(DirectionalLightDirection
    //DirectionalLightWorldToProjectionTransform, worldPosition);
    //float3 directionalLightProjectedPosition = directionalLightProjectedPositionTemp.xyz / directionalLightProjectedPositionTemp.w;
    //if (clamp(directionalLightProjectedPosition.x, -1.0, 1.0) == directionalLightProjectedPosition.x &&
    //    clamp(directionalLightProjectedPosition.y, -1.0, 1.0) == directionalLightProjectedPosition.y)
    //{
    //    float computedZ = directionalLightProjectedPosition.z;
    //    float shadowMapZ = gBufferedShadowMap.Sample(defaultSampler, 0.5f + float2(0.5f, -0.5f) * directionalLightProjectedPosition.xy);
    //    float bias = 0.001f;
    //    shadowFactor = (computedZ < shadowMapZ + bias);
    //    //shadowFactor = 1; // remove shadow
    //}
    
    //directionalLight *= shadowFactor;
    
    float3 emissiveAlbedo = albedo.rgb + emissive;
    float3 radiance = /*ambiance + directionalLight + */emissiveAlbedo;

    output.myColor = float4(radiance.xyz, 1.f);

    return output;
}