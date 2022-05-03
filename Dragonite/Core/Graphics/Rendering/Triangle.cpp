#include "Triangle.h"


using namespace CommonUtilities::Math;

Triangle::Triangle(ID3D11Device* aDevice, ID3D11DeviceContext* aDeviceContext, Vector3<float> aPosition, Vector3<float> aSize) : Render(aDevice, aDeviceContext)
{
	myObjectMatrix(1, 1) = aSize.x;
	myObjectMatrix(2, 2) = aSize.y;
	myObjectMatrix(3, 3) = aSize.z;
	myObjectMatrix.SetRow(4, { aPosition.x, aPosition.y, aPosition.z, 1.f });
}

MeshInfo Triangle::OnInit()
{
	MeshInfo info = MeshInfo();
	Vertex vertices[] =
	{
		Vector4<float>{-1.f, -1.f,  0, 1} *myObjectMatrix,Vector4<float>{1,0,0,1},
		Vector4<float>{0,      1.f,  0, 1} *myObjectMatrix,Vector4<float>{0,1,0,1},
		Vector4<float>{1.f,  -1.f,  0, 1} *myObjectMatrix,Vector4<float>{0,0,1,1},
	};

	unsigned int indices[] =
	{
		0,1,2
	};

	memcpy(info.myVerticies, vertices, sizeof(vertices));
	memcpy(info.myIndices, indices, sizeof(indices));

	info.myVerticiesAmm = 3;
	info.myIndicesAmm = 3;

	return info;
}
