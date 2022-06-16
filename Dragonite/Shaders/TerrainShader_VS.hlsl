#include "Utilities/ShaderUtilities.hlsli"


TerrainPixelInputType main(VertexInputType anInput)
{
    TerrainPixelInputType output;

	/*output.myPosition = anInput.myPosition;
	output.myColor = anInput.myColor;*/

    float4 vertexObjectPos = float4(anInput.myPosition, 1.0f);
    float4 vertexWorldPos = mul(myObjectMatrix, vertexObjectPos);
    float4 vertexClipPos = mul(myClipSpaceMatrix, vertexWorldPos);

	
    float4 vertexNormal = float4(anInput.myNormal.xyz, 0.f);
    float4 vertexWorldNormal = mul(myObjectMatrix, vertexNormal);
	
    output.myWorldPosition = vertexWorldPos;
    output.myPosition = vertexClipPos;
    output.myColor = anInput.myColor;
    output.myUV = anInput.myUV;
    output.myNormal = vertexWorldNormal;
    output.myBiNormal = mul(myObjectMatrix, float4(anInput.myBiNormal, 0.0f));
    output.myTangent = mul(myObjectMatrix, float4(anInput.myTangent, 0.0f));
    output.myClip = anInput.myPosition.y - mySize.y;
    //output.myTangent = mul(myObjectMatrix, float4(anInput.myTangent.xyz, 0.0f));
    //output.myBiNormal = mul(myObjectMatrix, float4(anInput.myBiNormal.xyz, 0.0f));
    
    return output;
}