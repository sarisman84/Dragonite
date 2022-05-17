#include "Utilities/ShaderUtilities.hlsli"

Texture2D aTexture : register(t0);
SamplerState aSampler : register(s0);

PixelOutput main(PixelInputType anInput)
{
    PixelOutput output;
    output.myColor = aTexture.Sample(aSampler, anInput.myUV.xy).rgba + myColor;

    return output;
}