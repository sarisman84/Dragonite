#include "Framework/ShaderStruct.hlsli"
#include "Framework/TextureDefs.hlsli"
#include "Framework/CBufferDefs.hlsli"

DefaultPixelOutput main(DefaultPixelInputType input)
{
    DefaultPixelOutput result;
	
   
    
    result.color = aTexture.Sample(aSampler, input.uv.xy).rgba;
    
    if(result.color.r == 0)
        discard;
    
    return result;

}