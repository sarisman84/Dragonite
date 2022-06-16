#include "Utilities/ShaderUtilities.hlsli"

#include "Utilities/PBRFunctions.hlsli"

Texture2D aTexture : register(t0);





PixelOutput main(PixelInputType anInput)
{
    PixelOutput result;
    
    float3 toEye = normalize(myCameraPosition.xyz - anInput.myWorldPosition.xyz);
    float fresnel = Fresnel_Schlick(
    float3(0.25f, 0.25f, 0.25f),
    float3(0.0f, 1.0f, 0.0f),
    toEye
    );
    
    float3 reflection = aTexture.Sample(defaultSampler, anInput.myPosition.xy / myResolution).rgb;
    result.myColor.rgb = fresnel * reflection;
    result.myColor.a = 1.f;
    
    return result;
}