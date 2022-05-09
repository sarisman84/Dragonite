#include "Utilities/ShaderUtilities.hlsli"



PixelOutput main(PixelInputType anInput)
{
	PixelOutput output;
	output.myColor = anInput.myColor;

	return output;
}