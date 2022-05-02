struct PixelInputType
{
	float4 myPosition : SV_POSITION;
	float4 myColor : COLOR;
};


float4 ColorPixelShader(PixelInputType anInput) : SV_TARGET
{
	return anInput.myColor;
}