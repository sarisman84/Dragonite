#include "Framework/ShaderStruct.hlsli"
#include "Framework/TextureDefs.hlsli"

DefaultPixelOutput main(DefaultPixelInputType input) 
{
    DefaultPixelOutput result;
	
    result.color = aTexture.Sample(aSampler, input.uv.xy).rgba;
    
    return result;

}