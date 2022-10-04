cbuffer FrameBuffer : register(b0)
{
    float4x4 worldToClipMatrix;
}

cbuffer ObjectBuffer : register(b1)
{
    float4x4 modelToWorld;
}

cbuffer RenderIDBuffer : register(b2)
{
    float id;
    float3 padding;
}