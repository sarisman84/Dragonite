#include "Util/ShaderStructs.hlsli"
#include "Util/CBufferDef.hlsli"

RenderIDOut main(DefaultPixelInputType input)
{
    RenderIDOut result;
	
    
    
    
    result.targetID = id;
    
    //if (id >= 0.25f)
    //    result.color = float4(1, 1, 1, 1);
    
    return result;

}