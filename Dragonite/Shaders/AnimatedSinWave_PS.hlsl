#include "Utilities/ShaderUtilities.hlsli"

PixelOutput main(PixelInputType anInput)
{
    PixelOutput output;
    output.myColor = anInput.myColor + sin((anInput.myPosition / myTimeDelta) * 2.f) * myTotalTime;

    return output;
}