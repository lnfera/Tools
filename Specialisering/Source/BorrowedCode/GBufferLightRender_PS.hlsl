#include "PBRFunctions.hlsli"
#include "PostprocessStructs.hlsli"
#include "Models_Basics.hlsli"

Texture2D gBufferPositionTexture : register(t6);
Texture2D gBufferAlbedoTexture : register(t7);
Texture2D gBufferNormalTexture : register(t8);
Texture2D gBufferMaterialTexture : register(t9);
//Texture2D gBufferAOAndCustom : register(t10);


PostProcessPixelOutput main(ModelVertexToPixel aInput)
{
    PostProcessPixelOutput output;
    
    float2 uv = aInput.position.xy / Resolution.xy;
    float3 worldPosition = gBufferPositionTexture.Sample(defaultSampler, uv).rgb /** 1000.f*/; //*1000 för att se bättre ut i debug
    float4 albedo = gBufferAlbedoTexture.Sample(defaultSampler, uv).rgba;
    float3 normal = normalize(2.f * gBufferNormalTexture.Sample(defaultSampler, uv).xyz - 1.f);
    float4 material = gBufferMaterialTexture.Sample(defaultSampler, uv);
    //float3 aoCustom = gBufferAOAndCustom.Sample(defaultSampler, uv);
    
    float3 toEye = normalize(cameraPosition.xyz - worldPosition.xyz);
    
    //if (albedo.a <= alphaTestThreshold)
    //{
    //    discard;
    //    output.myColor = float4(0.f, 0.f, 0.f, 0.f);
    //    return output;
    //}
    

    float ambientOcclusion = material.r;
    float metalness = material.b;
    float roughness = material.g;
    
    float3 specularColor = lerp((float3) 0.04f, albedo.rgb, metalness);
    float3 diffuseColor = lerp((float3) 0.00f, albedo.rgb, 1 - metalness);
    
    float3 lightCol = 0; // <- The sum of all point lights.
    for (unsigned int p = 0; p < NumberOfLights; p++)
    {
        switch (myPointLights[p].LightType)
        {
            case 0: // [PointLight]
            {
               
                    lightCol += EvaluatePointLight(
			    diffuseColor, specularColor, normal, roughness,
			    myPointLights[p].Color.rgb, myPointLights[p].Color.w, myPointLights[p].Range, myPointLights[p].Position.xyz,
			    toEye.xyz, worldPosition.xyz);
                break;
            }
            case 1: // [SpotLight]
            {
                lightCol += EvaluateSpotLight(
                    diffuseColor, specularColor, normal, roughness,
                    myPointLights[p].Color.rgb, myPointLights[p].Color.w, myPointLights[p].Range, myPointLights[p].Position.xyz,
                    myPointLights[p].SpotLightDir, myPointLights[p].OuterAngle, myPointLights[p].InnerAngle,
                    toEye.xyz, worldPosition.xyz);
                break;
            }
        }
    }
    
    
    output.myColor.rgb = lightCol.rgb;
    output.myColor.a = albedo.a;
    return output;

}