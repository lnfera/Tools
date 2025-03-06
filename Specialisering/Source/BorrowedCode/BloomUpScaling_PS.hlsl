#include "PostprocessStructs.hlsli"
#include "CBuffers.hlsli" 
PostProcessPixelOutput main(PostProcessVertexToPixel input)
{
    PostProcessPixelOutput returnValue;
    float2 pixelOffset = float2(ddx(input.myUV.x), ddy(input.myUV.y));
// Could have done one sample in the middle
// But that results in some artifacts. This pattern gives a much smoother result
    float3 p00 = FullscreenTexture1.Sample(defaultSampler, input.myUV + pixelOffset * float2(-1.0f, -1.0f)).rgb;
    float3 p01 = FullscreenTexture1.Sample(defaultSampler, input.myUV + pixelOffset * float2(-1.0f, 1.0f)).rgb;
    float3 p10 = FullscreenTexture1.Sample(defaultSampler, input.myUV + pixelOffset * float2(1.0f, -1.0f)).rgb;
    float3 p11 = FullscreenTexture1.Sample(defaultSampler, input.myUV + pixelOffset * float2(1.0f, 1.0f)).rgb;
    returnValue.myColor.rgb = 0.25f * (p00 + p01 + p10 + p11);
    returnValue.myColor.a = BloomBlending; // add to a constant buffer, expose to artists for control! 
    return returnValue;
}