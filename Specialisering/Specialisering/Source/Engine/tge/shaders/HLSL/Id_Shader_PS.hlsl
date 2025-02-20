#include "../common.hlsli"

SamplerState SampleType;
Texture2D shaderTextures[2] : register(t1);

uint2 main(ModelVertexToPixel aInput) : SV_Target
{
    float4 Diffuse = shaderTextures[0].Sample(SampleType, aInput.texCoord0.xy);
    if (Diffuse.a <= 0)
    {
        discard;
    }
    
    return uint2(objectId, selectionId);
}