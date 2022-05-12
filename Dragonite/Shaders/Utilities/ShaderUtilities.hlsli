struct PixelInputType
{
	float4 myPosition : SV_POSITION;
	float4 myColor : COLOR;
};

struct VertexInputType
{
	float4 myPosition: POSITION;
	float4 myColor : COLOR;
};


struct PixelOutput
{
    float4 myColor : SV_TARGET;
};

cbuffer FrameBuffer : register(b0)
{
    float4x4 myClipSpaceMatrix;
}

cbuffer ObjectBuffer : register(b1)
{
    float4x4 myObjectMatrix;
}
	