#pragma once
//
//#ifndef _MODELCLASS_H_
//#define _MODELCLASS_H_

#include "../CommonUtilities.h"
#include <wrl/client.h>



#pragma comment(lib, "d3d11.lib")


using Microsoft::WRL::ComPtr;
using CommonUtilities::Math::Vector3;
using CommonUtilities::Math::Vector4;

struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct ID3D11RenderTargetView;
struct ID3D11Buffer;


class Model
{
private:
	struct VertexType
	{
		Vector3<float> myPosition;
		Vector4<float> myColor;
	};
public:
	Model();
	Model(const Model& aModel);
	~Model();

	bool Initialize(ComPtr<ID3D11Device> aDevice);
	void Shutdown();
	void Render(ComPtr<ID3D11DeviceContext> aContext);

	int GetIndexCount();

private:
	bool InitializeBuffers(ComPtr<ID3D11Device> aDevice);
	void ShutdownBuffers();
	void RenderBuffers(ComPtr<ID3D11DeviceContext> aContext);

private:
	ComPtr<ID3D11Buffer> myVertexBuffer, myIndexBuffer;
	int myVertexCount, myIndexCount;
};

