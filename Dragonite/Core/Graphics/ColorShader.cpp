#include "ColorShader.h"
#include <d3d11.h>
#include <d3d10.h>
#include <d3dcompiler.h>

ColorShader::ColorShader()
{
}

ColorShader::ColorShader(const ColorShader& anOther)
{
}

ColorShader::~ColorShader()
{
}

bool ColorShader::Initialize(ComPtr<ID3D11Device> aDevice, HWND aWindowsHandle)
{
	bool result;

	result = InitializeShader(aDevice, aWindowsHandle, L"Color_VS.hlsl", L"Color_PS.hlsl");
	if (!result)
		return false;

	return true;
}

void ColorShader::Shutdown()
{
	ShutdownShader();
	return;

}

bool ColorShader::Render(ComPtr<ID3D11DeviceContext> aContext, int anIndexCount, Matrix4x4<float> aWorldMatrix, Matrix4x4<float> aViewMatrix, Matrix4x4<float> aProjectionMatrix)
{
	bool result;

	result = SetShaderParameters(aContext, aWorldMatrix, aViewMatrix, aProjectionMatrix);
	if (!result)
		return false;

	RenderShader(aContext, anIndexCount);
	return true;
}

bool ColorShader::InitializeShader(ComPtr<ID3D11Device> aDevice, HWND aWindowsHandle, const wchar_t* aVertexShaderPath, const wchar_t* aPixelShaderPath)
{
	HRESULT result;
	ComPtr<ID3D10Blob> errorMessage;
	ComPtr<ID3D10Blob> vertexShaderBuffer;
	ComPtr<ID3D10Blob> pixelShaderBuffer;

	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;

	result = D3DCompileFromFile(aVertexShaderPath, nullptr, nullptr, "ColorVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &vertexShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		if (errorMessage)
			OutputShaderErrorMessage(errorMessage, aWindowsHandle, aVertexShaderPath);
		else
			MessageBox(aWindowsHandle, aVertexShaderPath, L"Missing Shader File (Vertex Shader)", MB_OK);
		return false;
	}

	result = D3DCompileFromFile(aPixelShaderPath, nullptr, nullptr, "ColorPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &pixelShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		if (errorMessage)
			OutputShaderErrorMessage(errorMessage, aWindowsHandle, aPixelShaderPath);
		else
			MessageBox(aWindowsHandle, aPixelShaderPath, L"Missing Shader File (Pixel Shader)", MB_OK);
		return false;
	}



	result = aDevice->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &myVertexShader);
	if (FAILED(result)) return false;

	result = aDevice->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &myPixelShader);
	if (FAILED(result)) return false;


	// Now setup the layout of the data that goes into the shader.
	// This setup needs to match the VertexType stucture in the ModelClass and in the shader.
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "COLOR";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	result = aDevice->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &myLayout);
	if (FAILED(result)) return false;

	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	result = aDevice->CreateBuffer(&matrixBufferDesc, NULL, &myMatrixBuffer);
	if (FAILED(result)) return false;



	return true;
}

void ColorShader::ShutdownShader()
{
}

void ColorShader::OutputShaderErrorMessage(ComPtr<ID3D10Blob>, HWND aWindowsHandle, const wchar_t*)
{
}

bool ColorShader::SetShaderParameters(ComPtr<ID3D11DeviceContext> aContext, Matrix4x4<float> aWorldMatrix, Matrix4x4<float> aViewMatrix, Matrix4x4<float> aProjectionMatrix)
{
	return false;
}

void ColorShader::RenderShader(ComPtr<ID3D11DeviceContext> aContext, int)
{
}
