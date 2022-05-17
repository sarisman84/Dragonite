struct PixelInputType
{
    float4 myPosition : SV_POSITION;
    float4 myWorldPosition : POSITION;
    float4 myColor : COLOR;
    float2 myUV : TEXCOORD;
};

struct VertexInputType
{
    float4 myPosition : POSITION;
    float4 myColor : COLOR;
    float2 myUV : TEXCOORD;
};


struct PixelOutput
{
    float4 myColor : SV_TARGET;
};

cbuffer FrameBuffer : register(b0)
{
    float4x4 myClipSpaceMatrix;
    float myTimeDelta;
    float myTotalTime;
    float2 myGarbage;
}

cbuffer ObjectBuffer : register(b1)
{
    float4x4 myObjectMatrix;
}

cbuffer MaterialBuffer : register(b2)
{
    float4 myColor;
}

