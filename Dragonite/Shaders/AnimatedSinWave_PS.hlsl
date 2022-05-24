#include "Utilities/ShaderUtilities.hlsli"

Texture2D aTexture : register(t0);
SamplerState aSampler : register(s0);

PixelOutput main(PixelInputType anInput)
{
    float4 dirColor = myMaterialColor * myDirLightColor * max(0.0f, dot(myLightDirection, anInput.myNormal));
    float4 ambientColor = myMaterialColor * myAmbientColor;
    
    PixelOutput output;
    output.myColor = aTexture.Sample(aSampler, (anInput.myPosition.xy + float2(1.0f, 1.0f) * myTimeDelta)).rgba * ambientColor * dirColor;

    return output;
}