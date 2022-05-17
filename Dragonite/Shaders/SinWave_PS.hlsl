#include "Utilities/ShaderUtilities.hlsli"

Texture2D aTexture : register(t0);
SamplerState aSampler : register(s0);

PixelOutput main(PixelInputType anInput)
{
    PixelOutput output;
    output.myColor = sin((anInput.myPosition / 2)) * anInput.myColor;

    return output;
}