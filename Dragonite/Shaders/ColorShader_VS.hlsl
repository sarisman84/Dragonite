#include "Utilities/ShaderUtilities.hlsli"

PixelInputType main(VertexInputType anInput)
{
	PixelInputType output;

	/*output.myPosition = anInput.myPosition;
	output.myColor = anInput.myColor;*/

    float4 vertexObjectPos = anInput.myPosition;
    float4 vertexWorldPos = mul(myObjectMatrix, vertexObjectPos);
	float4 vertexClipPos = mul(myClipSpaceMatrix, vertexWorldPos);

	output.myPosition = vertexClipPos;
    output.myColor = anInput.myColor;

	return output;
}