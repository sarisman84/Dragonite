#include "Utilities/ShaderUtilities.hlsli"

Texture2D aTexture : register(t0);
SamplerState aSampler : register(s0);

PixelOutput main(PixelInputType anInput)
{
    PixelOutput output;
    output.myColor = aTexture.Sample(aSampler, (anInput.myPosition.xy + float2(1.0f, 1.0f)  * myTimeDelta)).rgba + myColor;

    return output;
}