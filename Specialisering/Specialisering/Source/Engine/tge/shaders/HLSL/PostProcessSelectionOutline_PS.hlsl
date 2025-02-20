#include "../PostprocessStructs.hlsli"

Texture2D<uint2> FullscreenTextureUint : register(t0);

PostProcessPixelOutput main(PostProcessVertexToPixel input)
{
	
	
    uint selection[3][3];
	
    bool hasDifference = false;
	
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            selection[i][j] = FullscreenTextureUint.Load(int3((int) input.myPosition.x - 1 + i, (int) input.myPosition.y - 1 + j, 0)).g;
	
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 2; j++)
            if (selection[i][j] != selection[i][j + 1])
                hasDifference = true;

    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 3; j++)
            if (selection[i][j] != selection[i + 1][j])
                hasDifference = true;
    
	
    if (!hasDifference)
        discard;
    
    PostProcessPixelOutput returnValue;
    returnValue.myColor.rgb = float3(1.f, 0.1f, 0.f);
    returnValue.myColor.a = 1.f;
    return returnValue;
}