#pragma once
#include "../PostprocessStructs.hlsli"
#include "../Common.hlsli"
#include "../PBRFunctions.hlsli"

Texture2D gBufferPositionTexture : register(t1);
Texture2D gBufferAlbedoTexture : register(t2);
Texture2D gBufferNormalTexture : register(t3);
Texture2D gBufferMaterialTexture : register(t4);
Texture2D gBufferAOAndCustom : register(t5);

PostProcessPixelOutput main(ModelVertexToPixel input)
{
    PostProcessPixelOutput output;
	
    float2 uv = input.position.xy / Resolution.xy;
    float3 worldPosition = gBufferPositionTexture.Sample(defaultSampler, uv).rgb /** 1000.f*/; //*1000 för att se bättre ut i debug
    float4 albedo = gBufferAlbedoTexture.Sample(defaultSampler, uv).rgba;
    float3 normal = normalize(gBufferNormalTexture.Sample(defaultSampler, uv).xyz);
    float4 material = gBufferMaterialTexture.Sample(defaultSampler, uv);
	
    float3 toEye = normalize(CameraPosition.xyz - worldPosition.xyz);

    float ambientOcclusion = material.r;
    float metalness = material.b;
    float roughness = material.g;
    
    float3 specularColor = lerp((float3) 0.04f, albedo.rgb, metalness);
    float3 diffuseColor = lerp((float3) 0.00f, albedo.rgb, 1 - metalness);
    
    float3 lightCol = 0; // <- The sum of all point lights.
    for (unsigned int p = 0; p < NumberOfLights; p++)
    {
        lightCol += EvaluatePointLight(
			    diffuseColor, specularColor, normal, roughness,
			    myPointLights[p].Color.rgb, myPointLights[p].Color.w, myPointLights[p].Range, myPointLights[p].Position.xyz,
			    toEye.xyz, worldPosition.xyz);
    }
    
    output.myColor.rgb = lightCol.rgb;
    //output.myColor.rgb = float4(1, 0, 0, 1000);
    return output;
}
