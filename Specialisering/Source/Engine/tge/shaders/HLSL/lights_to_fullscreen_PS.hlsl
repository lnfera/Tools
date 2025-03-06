#pragma once
#include "../PostprocessStructs.hlsli"
#include "../Common.hlsli"
#include "../PBRFunctions.hlsli"

Texture2D gBufferPositionTexture	: register(t1);
Texture2D gBufferAlbedoTexture		: register(t2);
Texture2D gBufferNormalTexture		: register(t3);
Texture2D gBufferMaterialTexture	: register(t4);
Texture2D gBufferAOAndCustom		: register(t5);

PostProcessPixelOutput main(PostProcessVertexToPixel input)
{
	PostProcessPixelOutput output;
    output.myColor = float4(0,0,0,0);
	float2 uv = input.myPosition.xy / Resolution.xy;

	
	return output;
}
