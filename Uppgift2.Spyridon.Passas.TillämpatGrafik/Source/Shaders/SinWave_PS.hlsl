#include "Utilities/ShaderUtilities.hlsli"

PixelOutput main(PixelInputType anInput)
{
    PixelOutput output;
    output.myColor = sin((anInput.myPosition / 2)) * anInput.myColor;

    return output;
}