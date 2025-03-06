#include "PostprocessStructs.hlsli"
#include "CBuffers.hlsli" 

float3 s_curve(float3 x)
{
    float a = 2.51f;
    float b = 0.03f;
    float c = 2.43f;
    float d = 0.59f;
    float e = 0.14f;
    return clamp((x * (a * x + b)) / (x * (c * x + d) + e), 0.0, 1.0);
}


float3 tonemap_s_gamut3_cine(float3 c)
{
    // based on Sony's s gamut3 cine
    float3x3 fromSrgb = float3x3(
        +0.6456794776, +0.2591145470, +0.0952059754,
        +0.0875299915, +0.7596995626, +0.1527704459,
        +0.0369574199, +0.1292809048, +0.8337616753);

    float3x3 toSrgb = float3x3(
        +1.6269474099, -0.5401385388, -0.0868088707,
        -0.1785155272, +1.4179409274, -0.2394254004,
        +0.0444361150, -0.1959199662, +1.2403560812);

    return mul(toSrgb, s_curve(mul(fromSrgb, c)));
}

PostProcessPixelOutput main(PostProcessVertexToPixel input)
{
    PostProcessPixelOutput returnValue;
    
        
    float3 bloomColor = FullscreenTexture1.Sample(defaultSampler, input.myUV).rgb;
    float3 uneditedColor = FullscreenTexture2.Sample(defaultSampler, input.myUV).rgb;
    float3 depthMap = FullscreenTexture3.Sample(defaultSampler, input.myUV).rgb;
    
    
    float3 color;
    
    float depth = depthMap.r;
    
    color.r = lerp(uneditedColor.r, bloomColor.r, depth);
    color.g = lerp(uneditedColor.g, bloomColor.g, depth);
    color.b = lerp(uneditedColor.b, bloomColor.b, depth);
    
    returnValue.myColor.rgb = color.rgb;
    
    //saturation
        float luminance = dot(float3(0.2126, 0.7152, 0.0722), color);
    color = luminance + Saturation * (color - luminance);
    
    //exposure
    color = exp2(Exposure) * color;
    
    //contrast
    color = 0.18f * pow(color / 0.18f, Contrast);
    //tint
    color = color * Tint;
    // blackpoint
    color = color - BlackPoint;
    color = max(0.0f, color);
    
    returnValue.myColor.rgb = tonemap_s_gamut3_cine(color);
    returnValue.myColor.a = 1.0f;
    return returnValue;
}