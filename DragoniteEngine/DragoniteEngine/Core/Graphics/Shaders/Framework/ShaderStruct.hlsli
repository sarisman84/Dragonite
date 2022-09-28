struct DefaultVertexInputType
{
    float4 position : POSITION;
    float2 uv : TEXCOORD;
};

struct DefaultPixelInputType
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

struct DefaultPixelOutput
{
    float4 color : SV_TARGET;
};