#include "Framework/ShaderStruct.hlsli"
#include "Framework/TextureDefs.hlsli"
#include "Framework/CBufferDefs.hlsli"

DefaultPixelOutput main(DefaultPixelInputType input)
{
    DefaultPixelOutput result;
	
    unsigned int id = (unsigned int) aTexture.Sample(aUINTSampler, input.uv.xy).r;
    

    
    if (id == 0)
        discard;
    
    
    result.color.r = (float) id / 11.0f;
    
    return result;

}