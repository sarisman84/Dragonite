#include "Framework/ShaderStruct.hlsli"
#include "Framework/TextureDefs.hlsli"
#include "Framework/CBufferDefs.hlsli"

DefaultPixelOutput main(SimplePixelInput input)
{
    DefaultPixelOutput result;
	
    result.color = float4(id, 0.0f, 0.0f, 1.0f);
    
    return result;

}