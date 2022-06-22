#include "Utilities/ShaderUtilities.hlsli"

Texture2D aTexture : register(t0);
Texture2D aNormalMap : register(t1);


SamplerState aSampler : register(s0);


PixelOutput main(PixelInputType anInput)
{
    float4 normalSample = aNormalMap.Sample(aSampler, anInput.myUV.xy).rgba;

    float3 textureNormal = GetNormal(normalSample);
    
    //taken from this: https://stackoverflow.com/a/18061041
    float3 n = (anInput.myNormal.xyz);
    float3 t = (anInput.myTangent.xyz);
    float3 b = (anInput.myBiNormal.xyz);
    
    //textureNormal = float3(0.f, 1.f, 0.f);
    
    float3x3 tangentSpace = float3x3(t, b, n);
    float3 normal = normalize(mul(textureNormal, tangentSpace));
    
    
    
    float4 dirColor = myMaterialColor * myDirLightColor * max(0.0f, dot(-myLightDirection, float4(normal, 0.f)));
    float4 ambientColor = myMaterialColor * myAmbientColor;
    
    PixelOutput output;
    output.myColor = aTexture.Sample(aSampler, anInput.myUV.xy).rgba + ambientColor + dirColor;
    
    //output.myColor.rgb = 0.5f + 0.5f * normal;
    


    
    
    return output;
}