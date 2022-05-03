#include "Utilities/ShaderUtilities.hlsli"

PixelInputType main(VertexInputType anInput)
{
	PixelInputType output;

	output.myPosition = anInput.myPosition;
	output.myColor = anInput.myColor;

	return output;
}