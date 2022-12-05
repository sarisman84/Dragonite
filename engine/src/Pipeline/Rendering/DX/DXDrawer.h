#pragma once
#include "Pipeline/Rendering/RenderAdapter.h"
#include "DXIncludes.h"


struct ID3D11Device;
struct ID3D11DeviceContext;

namespace Dragonite
{
	struct RTView : public IContent
	{
		RTView();
		~RTView() override;
		inline ID3D11RenderTargetView*& Data() { return myRenderTargetView; }
	private:
		ID3D11RenderTargetView* myRenderTargetView;
	};

	struct DSView : public IContent
	{
		DSView();
		~DSView() override;
		inline ID3D11DepthStencilView*& Data() { return myDepthStencilView; }
	private:
		ID3D11DepthStencilView* myDepthStencilView;
	};

	struct DXDrawer : IDrawer
	{
		DXDrawer();
		~DXDrawer() override;
		void Present(bool aVSyncState) override;
		void Init(HWND anInstance) override;

		void SetRenderTarget(void* aTargetBuffer, void* aDepthBuffer = nullptr) override;
		void ClearRenderTarget(void* aTargetBuffer, void* aDepthBuffer = nullptr) override;


		inline IDXGISwapChain* SwapChain() noexcept { return mySwapChain; }
		inline ID3D11Device* Device() noexcept { return myDevice; }
		inline ID3D11DeviceContext* Context() noexcept { return myContext; }

	private:
		IDXGISwapChain* mySwapChain;
		ID3D11Device* myDevice;
		ID3D11DeviceContext* myContext;

		// Inherited via IDrawer
	};

	template<typename Type>
	struct DXBuffer : public IContent
	{
		DXBuffer(ID3D11Device* aDevice, ID3D11DeviceContext* aContext);
		~DXBuffer();
		void Modify(Type* someData);
		void PSBind(UINT aSlot);
		void VSBind(UINT aSlot);
	private:
		ID3D11Buffer* myBuffer;
		ID3D11Device* myDevicePtr;
		ID3D11DeviceContext* myContextPtr;
	};

	void InitializeBuffer(ID3D11Device* aDevice, ID3D11Buffer** aBuffer, const size_t aTypeSize);
	void EditBuffer(ID3D11DeviceContext* aContext, UINT aSlot, ID3D11Buffer* aBuffer, const size_t aTypeSize, void* someData);
	void BindBuffer(ID3D11DeviceContext* aContext, UINT aBindType, ID3D11Buffer** aBuffer, UINT aSlot);

	template<typename Type>
	inline DXBuffer<Type>::DXBuffer(ID3D11Device* aDevice, ID3D11DeviceContext* aContext) : myDevicePtr(aDevice), myContextPtr(aContext)
	{
		Dragonite::InitializeBuffer(aDevice, &myBuffer, sizeof(Type));
	}
	template<typename Type>
	inline DXBuffer<Type>::~DXBuffer()
	{
		if (myBuffer)
			myBuffer->Release();
		myBuffer = nullptr;
	}
	template<typename Type>
	inline void DXBuffer<Type>::Modify(Type* someData)
	{
		Dragonite::EditBuffer(myContextPtr, 0, myBuffer, sizeof(Type), someData);
	}
	template<typename Type>
	inline void DXBuffer<Type>::PSBind(UINT aSlot)
	{
		Dragonite::BindBuffer(myContextPtr, 1, &myBuffer, aSlot);
	}
	template<typename Type>
	inline void DXBuffer<Type>::VSBind(UINT aSlot)
	{
		Dragonite::BindBuffer(myContextPtr, 0, &myBuffer, aSlot);
	}
}