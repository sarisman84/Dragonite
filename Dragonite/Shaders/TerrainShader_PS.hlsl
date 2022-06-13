#include "Utilities/ShaderUtilities.hlsli"
#include "Utilities/PBRFunctions.hlsli"

Texture2D aTexture1 : register(t0);
Texture2D aTexture2 : register(t1);
Texture2D aTexture3 : register(t2);


Texture2D aNormalTexture1 : register(t3);
Texture2D aNormalTexture2 : register(t4);
Texture2D aNormalTexture3 : register(t5);


Texture2D aMaterialTexture1 : register(t6);
Texture2D aMaterialTexture2 : register(t7);
Texture2D aMaterialTexture3 : register(t8);

TextureCube anEnviromentalTexture : register(t9);

 




float4 DrawTexture(PixelInputType anInput, Texture2D anAlbedoMap, Texture2D aNormalMap, Texture2D aMaterialTexture, float3x3 someTangentSpace, float3 anEyePosition)
{
    float3 material = aMaterialTexture.Sample(defaultSampler, anInput.myUV).rgb;
    
    float metallic = material.r;
    float roughness = material.g;
    float emissive = material.b;
   
 
    
    
    float4 normalSample = aNormalMap.Sample(defaultSampler, anInput.myUV).rgba;
    
    
    float ambientOcclusion = normalSample.b;
    
    
    
    
    float3 textureNormal = GetNormal(normalSample);
    
  
    
    
    float3 normal = normalize(mul(textureNormal, someTangentSpace));
    
    //float4 ambientColor = myMaterialColor * myAmbientColor;
    //float4 dirColor = myMaterialColor * myDirLightColor * max(0.1f, dot(-myLightDirection, float4(normal, 0.f)));
    
    float4 albedo = anAlbedoMap.Sample(defaultSampler, anInput.myUV).rgba;
    
    //if (albedo.a < 0.5f)
    //{
    //    discard;
    //    return float4(0.0f, 0.0f, 0.0f, 0.0f);

    //}
    
    float3 specularColor = lerp((float3) 0.04f, albedo.rgb, metallic);
    float3 diffuseColor = lerp((float3) 0.00f, albedo.rgb, 1 - metallic);
    
    //specularColor = 0;
    //diffuseColor = 0;
    
    float3 ambiance = EvaluateAmbiance(
		anEnviromentalTexture, normal, anInput.myNormal.xyz,
		anEyePosition, roughness,
		ambientOcclusion, diffuseColor, specularColor
	);
    
    float3 directionalLight = EvaluateDirectionalLight(
		diffuseColor, specularColor, normal, roughness,
		myDirLightColor.xyz, -myLightDirection.xyz, anEyePosition.xyz
	);
    
    
    
    float3 radiance = ambiance + directionalLight;
    
    return float4(tonemap_s_gamut3_cine(radiance), 1.f);
    
}



PixelOutput main(PixelInputType anInput)
{
    
    float3 toEye = normalize(myCameraPosition.xyz - anInput.myWorldPosition.xyz);
    
    //taken from this: https://stackoverflow.com/a/18061041
    float3 n = normalize(anInput.myNormal.xyz);
    float3 t = normalize(anInput.myTangent.xyz);
    float3 b = normalize(anInput.myBiNormal.xyz);
    
    //textureNormal = float3(0.f, 1.f, 0.f);
    
    float3x3 tangentSpace = float3x3(t, b, n);
 
    
    float slopeBlend = smoothstep(0.7f, 1.0f, normalize(anInput.myNormal).y);
    float heightBlend = smoothstep(-0.05f, 0.25f, anInput.myWorldPosition.y);
    
    
    
    PixelOutput output;
    
    float4 color1 = DrawTexture(anInput, aTexture1, aNormalTexture1, aMaterialTexture1, tangentSpace, toEye);
    float4 color2 = DrawTexture(anInput, aTexture2, aNormalTexture2, aMaterialTexture2, tangentSpace, toEye);
    float4 color3 = DrawTexture(anInput, aTexture3, aNormalTexture3, aMaterialTexture3, tangentSpace, toEye);
    
    output.myColor = lerp(color2, lerp(color1, color3, heightBlend), slopeBlend);
    
    //output.myColor.rgb = 0.5f + 0.5f * normal;
    

    
    
    
    return output;
}