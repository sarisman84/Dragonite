#include "Framework/ShaderStruct.hlsli"
#include "Framework/CBufferDefs.hlsli"

DefaultPixelInputType main(VertexIndex input)
{
    DefaultPixelInputType output;
    
    float4 pos[3] =
    {
        float4(-1.0f, -1.0f, 0.0f, 1.0f),
		float4(-1.0f, 3.0f, 0.0f, 1.0f),
		float4(3.0f, -1.0f, 0.0f, 1.0f)
    };

    float2 uv[3] =
    {
        float2(0.0f, 1.0f),
		float2(0.0f, -1.0f),
		float2(2.0f, 1.0f)
    };

    output.position = pos[input.index];
    output.uv = uv[input.index];
  
    return output;
}