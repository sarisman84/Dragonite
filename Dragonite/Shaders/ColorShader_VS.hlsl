#include "Utilities/ShaderUtilities.hlsli"

PixelInputType main(VertexInputType anInput)
{
	PixelInputType output;

	/*output.myPosition = anInput.myPosition;
	output.myColor = anInput.myColor;*/

    float4 vertexObjectPos = float4(anInput.myPosition, 1.0f);
    float4 vertexWorldPos = mul(myObjectMatrix, vertexObjectPos);
	float4 vertexClipPos = mul(myClipSpaceMatrix, vertexWorldPos);

	
    float4 vertexNormal = float4(anInput.myNormal, 0.f);
    float4 vertexWorldNormal = mul(myObjectMatrix, vertexNormal);
	
    output.myWorldPosition = vertexWorldPos;
	output.myPosition = vertexClipPos;
    output.myColor = anInput.myColor;
    output.myUV = anInput.myUV;
    output.myNormal = vertexWorldNormal;
    
	return output;
}