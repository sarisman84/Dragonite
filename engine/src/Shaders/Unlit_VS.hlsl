#include "Util/ShaderStructs.hlsli"
#include "Util/CBufferDef.hlsli"

DefaultPixelInputType main(DefaultVertexInputType input)
{
    DefaultPixelInputType output;
    
    float4 vertexObjectPos = input.position;
    float4 vertexWorldPos = mul(modelToWorld, vertexObjectPos);
    float4 vertexClipPos = mul(worldToClipMatrix, vertexWorldPos);
    
    output.position = vertexClipPos;
    output.uv = input.uv;
    
    return output;
}