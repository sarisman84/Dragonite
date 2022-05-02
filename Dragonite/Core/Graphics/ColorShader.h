#pragma once
#include <wrl/client.h>
#include "../CommonUtilities.h"

using CommonUtilities::Math::Matrix4x4;
using Microsoft::WRL::ComPtr;

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D10Blob;
struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11InputLayout;
struct ID3D11Buffer;

class ColorShader
{
private:
	struct MatrixBufferType
	{
		Matrix4x4<float> myWorldMatrix;
		Matrix4x4<float> myViewMatrix;
		Matrix4x4<float> myProjectionMatrix;
	};
public:
	ColorShader();
	ColorShader(const ColorShader& anOther);
	~ColorShader();

	bool Initialize(ComPtr<ID3D11Device> aDevice, HWND aWindowsHandle);
	void Shutdown();
	bool Render(ComPtr<ID3D11DeviceContext> aContext, int anIndexCount, Matrix4x4<float> aWorldMatrix, Matrix4x4<float> aViewMatrix, Matrix4x4<float> aProjectionMatrix);


private:
	bool InitializeShader(ComPtr<ID3D11Device> aDevice, HWND aWindowsHandle, const wchar_t* aVertexShaderPath, const wchar_t* aPixelShaderPath);
	void ShutdownShader();
	void OutputShaderErrorMessage(ComPtr<ID3D10Blob>, HWND aWindowsHandle, const wchar_t*);

	bool SetShaderParameters(ComPtr<ID3D11DeviceContext> aContext, Matrix4x4<float> aWorldMatrix, Matrix4x4<float> aViewMatrix, Matrix4x4<float> aProjectionMatrix);
	void RenderShader(ComPtr<ID3D11DeviceContext> aContext, int);

private:
	ComPtr<ID3D11VertexShader> myVertexShader;
	ComPtr<ID3D11PixelShader> myPixelShader;
	ComPtr<ID3D11InputLayout> myLayout;
	ComPtr<ID3D11Buffer> myMatrixBuffer;
};

