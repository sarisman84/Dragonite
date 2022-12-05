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


struct VertexIndex
{
    unsigned int index : SV_VertexID;
};

struct RenderIDOut
{
    unsigned int targetID : SV_TARGET;
};


Texture2D aTexture : register(t0);
SamplerState aSampler : register(s0);
SamplerState aUINTSampler : register(s1);


