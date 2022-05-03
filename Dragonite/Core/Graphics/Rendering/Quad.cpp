#include "Quad.h"
using namespace CommonUtilities::Math;

Quad::Quad(ID3D11Device* aDevice, ID3D11DeviceContext* aDeviceContext, Vector3<float> aPosition, Vector3<float> aSize) : Render(aDevice, aDeviceContext)
{
	myObjectMatrix(1, 1) = aSize.x;
	myObjectMatrix(2, 2) = aSize.y;
	myObjectMatrix(3, 3) = aSize.z;
	myObjectMatrix.SetRow(4, { aPosition.x, aPosition.y, aPosition.z, 1.f });
}

MeshInfo Quad::OnInit()
{
	MeshInfo info = MeshInfo();
	info.myVerticiesAmm = 4;
	info.myIndicesAmm = 6;

	Vertex vertices[] =
	{
		{Vector4<float>(1.f, 1.f, 1.f, 1.f) * myObjectMatrix, Vector4<float>(1.f, 0.f, 0.f, 1.f)},
		{Vector4<float>(-1.f, 1.f, 1.f, 1.f) * myObjectMatrix, Vector4<float>(0.f, 1.f, 0.f, 1.f)},
		{Vector4<float>(-1.f, -1.f, 1.f, 1.f) * myObjectMatrix, Vector4<float>(0.f, 0.f, 1.f, 1.f)},
		{Vector4<float>(1.f, -1.f, 1.f, 1.f) * myObjectMatrix, Vector4<float>(1.f, 0.f, 1.f, 1.f)},
	};

	unsigned int indices[] =
	{
		3,1,0,2,1,3
	};


	memcpy(info.myVerticies, vertices, sizeof(vertices));
	memcpy(info.myIndices, indices, sizeof(indices));


	myPixelShaderPath = "Shaders/SinWave_PS.cso";
	return info;
}
