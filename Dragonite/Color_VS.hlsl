cbuffer MatrixBuffer
{
	matrix myWorldMatrix;
	matrix myViewMatrix;
	matrix myProjectionMatrix;
};


struct VertexInputType
{
	float4 myPosition : POSITION;
	float4 myColor: COLOR;
};

struct PixelInputType
{
	float4 myPosition : SV_POSITION;
	float4 myColor: COLOR;
};

PixelInputType ColorVertexShader(VertexInputType anInput)
{
	PixelInputType output;

	anInput.myPosition.w = 1.0f;


	// Calculate the position of the vertex against the world, view, and projection matrices.
	output.myPosition = mul(anInput.myPosition, myWorldMatrix);
	output.myPosition = mul(output.myPosition, myViewMatrix);
	output.myPosition = mul(output.myPosition, myProjectionMatrix);

	// Store the input color for the pixel shader to use.
	output.myColor = anInput.myColor;

	return output;
}