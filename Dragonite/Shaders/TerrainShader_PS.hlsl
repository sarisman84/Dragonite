#include "Utilities/ShaderUtilities.hlsli"


Texture2D aTexture1 : register(t0);
Texture2D aTexture2 : register(t1);
Texture2D aTexture3 : register(t2);


Texture2D aNormalTexture1 : register(t3);
Texture2D aNormalTexture2 : register(t4);
Texture2D aNormalTexture3 : register(t5);


SamplerState aSampler : register(s0);



float4 DrawTexture(PixelInputType anInput, Texture2D anAlbedoMap, Texture2D aNormalMap, float3x3 someTangentSpace)
{
    float4 normalSample = aNormalMap.Sample(aSampler, anInput.myUV.xy).rgba;
    float3 textureNormal = GetNormal(normalSample);
    
    float3 normal = normalize(mul(textureNormal, someTangentSpace));
    
    float4 ambientColor = myMaterialColor * myAmbientColor;
    float4 dirColor = myMaterialColor * myDirLightColor * max(0.1f, dot(-myLightDirection, float4(normal, 0.f)));
    
    return anAlbedoMap.Sample(aSampler, anInput.myUV.xy).rgba * (dirColor + ambientColor);
    
}



PixelOutput main(PixelInputType anInput)
{
    //taken from this: https://stackoverflow.com/a/18061041
    float3 n = (anInput.myNormal.xyz);
    float3 t = (anInput.myTangent.xyz);
    float3 b = (anInput.myBiNormal.xyz);
    
    //textureNormal = float3(0.f, 1.f, 0.f);
    
    float3x3 tangentSpace = float3x3(t, b, n);
 
    
    float slopeBlend = smoothstep(0.7f, 1.f, anInput.myNormal.y);
    float heightBlend = smoothstep(25.0f, 150.f, anInput.myWorldPosition.y);
    
    
    
    PixelOutput output;
    
    float4 color1 = DrawTexture(anInput, aTexture1, aNormalTexture1, tangentSpace);
    float4 color2 = DrawTexture(anInput, aTexture2, aNormalTexture2, tangentSpace);
    float4 color3 = DrawTexture(anInput, aTexture3, aNormalTexture3, tangentSpace);
    
    output.myColor = lerp(color2, lerp(color1, color3, heightBlend), slopeBlend);
    
    //output.myColor.rgb = 0.5f + 0.5f * normal;
    


    
    
    return output;
}