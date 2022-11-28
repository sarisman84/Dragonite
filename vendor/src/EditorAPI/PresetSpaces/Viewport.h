#pragma once
#include "imgui/imgui.h"
#include "EditorAPI/Space/WindowSpace.h"

#include <functional>


struct ID3D11RenderTargetView;
struct ID3D11ShaderResourceView;



namespace ember
{
	class SceneView : public EmberWindow
	{
	public:
		SceneView(const std::function<void(ID3D11RenderTargetView*)>& aRenderCall);
		~SceneView() override;
		// Inherited via EmberWindow
		void OnGUI() override;
		void OnInit() override;
	private:
		std::function<void(ID3D11RenderTargetView*)> mySceneRenderCall;
		ID3D11RenderTargetView* myLocalRenderView;
		ID3D11ShaderResourceView* myShaderResourceView;

	private:
		float myWidth, myHeight;
	};

	//void InitRenderTarget(ID3D11Device* aDevice, const float aWidth, const float aHeight, ID3D11RenderTargetView** aTargetView, ID3D11ShaderResourceView** aShaderView);
	//void ReleaseElements(ID3D11RenderTargetView* aTargetView, ID3D11ShaderResourceView* aShaderView);

	//template<typename RenderCall>
	//struct Viewport : public GUISpace
	//{
	//	Viewport(const char* aName, const EmVec2& aViewportSize, RenderCall&& aRenderCall, EmberGUI* const anInterface);
	//	~Viewport() override 
	//	{
	//		ReleaseElements(myViewportView, myTextureData);
	//	}
	//	void Invoke(void* someExtraData) override;

	//private:

	//	RenderCall myRenderCall;
	//	ID3D11ShaderResourceView* myTextureData;
	//	ID3D11RenderTargetView* myViewportView;
	//	EmVec2 myViewportSize;
	//};



	//template<typename RenderCall>
	//EmberGUILayout::Viewport<RenderCall>::Viewport(const char* aName, const EmVec2& aViewportSize, RenderCall&& aRenderCall, EmberGUI* const anInterface) : 
	//	GUISpace(aName, anInterface), myRenderCall(aRenderCall)
	//{
	//	myViewportSize = aViewportSize;

	//	auto clientSize = myGUIInterface->GetViewportResolution();
	//	InitRenderTarget(myDevice, clientSize.x, clientSize.y, &myViewportView, &myTextureData);
	//}

	//template<typename RenderCall>
	//void EmberGUILayout::Viewport<RenderCall>::Invoke(void* someExtraData)
	//{
	//	ImGui::SetCurrentContext(myGUIInterface->GetIMGUIContext());
	//	myRenderCall((void*)myViewportView);
	//	ImGui::Image(myTextureData, ImVec2(myViewportSize.x, myViewportSize.y));

	//}


}






