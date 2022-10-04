#include "Framework/ShaderStruct.hlsli"
#include "Framework/CBufferDefs.hlsli"

SimplePixelInput main(SimpleVertexInput input)
{
    SimplePixelInput output;
    
    float4 vertexObjectPos = input.position;
    float4 vertexWorldPos = mul(modelToWorld, vertexObjectPos);
    float4 vertexClipPos = mul(worldToClipMatrix, vertexWorldPos);
    
    output.position = vertexClipPos;
    
    return output;
}