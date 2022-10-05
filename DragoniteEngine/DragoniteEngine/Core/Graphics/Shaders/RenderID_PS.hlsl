#include "Framework/ShaderStruct.hlsli"
#include "Framework/TextureDefs.hlsli"
#include "Framework/CBufferDefs.hlsli"

RenderIDOut main(DefaultPixelInputType input)
{
    RenderIDOut result;
	
    
    
    
    result.targetID = id;
    
    
    //if (id >= 0.25f)
    //    result.color = float4(1, 1, 1, 1);
    
    return result;

}