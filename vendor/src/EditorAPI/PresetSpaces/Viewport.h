#pragma once
#include "imgui/imgui.h"
#include "EditorAPI/Space/WindowSpace.h"

struct ID3D11RenderTargetView;
struct ID3D11ShaderResourceView;

void InitRenderTarget(ID3D11Device* aDevice, const float aWidth, const float aHeight, ID3D11RenderTargetView* aTargetView, ID3D11ShaderResourceView* aShaderView);
void SwitchRenderView(ID3D11DeviceContext* aContext, ID3D11RenderTargetView* aTargetView);

namespace EmberAPI
{
	template<typename RenderCall>
	class Viewport : public GUISpace
	{
	public:
		Viewport(ID3D11Device* aDevice, ID3D11DeviceContext* aContext, const char* aName, const float aWidth, const float aHeight, RenderCall&& aRenderCall);
		void Invoke() override;

	private:
		ID3D11RenderTargetView* myRenderTargetView;
		ID3D11ShaderResourceView* myShaderResourceView;
		RenderCall myRenderCall;
		float myWidth, myHeight;
	};



	template<typename RenderCall>
	inline Viewport<RenderCall>::Viewport(
		ID3D11Device* aDevice,
		ID3D11DeviceContext* aContext,
		const char* aName,
		const float aWidth,
		const float aHeight,
		RenderCall&& aRenderCall) : GUISpace(aDevice, aContext)
	{
		myName = aName;
		myWidth = aWidth;
		myHeight = aHeight;
		InitRenderTarget(myDevice, aWidth, aHeight, myRenderTargetView, myShaderResourceView);
	}

	template<typename RenderCall>
	inline void Viewport<RenderCall>::Invoke()
	{
		SwitchRenderView(myContext, myRenderTargetView);
		myRenderCall();
		ImGui::Image(myShaderResourceView, ImVec2(myWidth, myHeight));
	}

}






