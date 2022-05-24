#include "Utilities/ShaderUtilities.hlsli"

Texture2D aTexture : register(t0);
SamplerState aSampler : register(s0);

PixelOutput main(PixelInputType anInput)
{
    float4 dirColor = myMaterialColor * myDirLightColor * max(0.0f, dot(-myLightDirection, anInput.myNormal));
    float4 ambientColor = myMaterialColor * myAmbientColor;
    
    PixelOutput output;
    output.myColor = aTexture.Sample(aSampler, anInput.myUV.xy).rgba * ambientColor * dirColor;

    
    
    return output;
}