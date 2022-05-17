#include "Utilities/ShaderUtilities.hlsli"

Texture2D aTexture : register(t0);
SamplerState aSampler : register(s0);

PixelOutput main(PixelInputType anInput)
{
    PixelOutput output;
    output.myColor = trunc(sin((anInput.myPosition * myTimeDelta) / myTimeDelta) * myTotalTime * anInput.myColor);

    return output;
}