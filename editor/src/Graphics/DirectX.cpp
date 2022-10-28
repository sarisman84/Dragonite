//#include "DirectX.h"
//
//#include <d3d11.h>
//
//HRESULT DirectX::Initialize(HWND anInstance)
//{
//	if (!anInstance) return E_INVALIDARG;
//	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
//	swapChainDesc.BufferCount = 1;
//	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
//	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
//	swapChainDesc.OutputWindow = anInstance;
//	swapChainDesc.SampleDesc.Count = 1;
//	swapChainDesc.Windowed = false;
//
//	UINT creationFlags = 0;
//
//	return D3D11CreateDeviceAndSwapChain(
//		nullptr,
//		D3D_DRIVER_TYPE_HARDWARE,
//		nullptr,
//		creationFlags,
//		nullptr,
//		0,
//		D3D11_SDK_VERSION,
//		&swapChainDesc,
//		&SwapChain,
//		&Device,
//		nullptr,
//		&Context
//	);
//}
