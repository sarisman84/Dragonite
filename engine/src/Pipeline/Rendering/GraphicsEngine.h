#pragma once

#include <wtypes.h>
#include <vector>
#include <memory>
#include <algorithm>

#include <unordered_map>


namespace Dragonite
{
	struct IDrawer;

	struct RenderCall
	{
		RenderCall(unsigned int aVSID, unsigned int aPSID, unsigned int anIID)
		{
			myVSID = aVSID;
			myPSID = aPSID;
			myIID = anIID;
		}
		virtual void Render() = 0;
		unsigned int myVSID, myPSID, myIID;
	};


	template<typename Call>
	struct DrawCall : public RenderCall
	{
	public:
		DrawCall(Call&& aRenderCall, unsigned int aVSID, unsigned int aPSID, unsigned int anIID) : RenderCall(aVSID, aPSID, anIID)
		{
			myRenderCall = aRenderCall;
		}
		void Render() override {
			myRenderCall(this);
		}
	private:
		Call myRenderCall;
	};


	class GraphicsEngine
	{
	public:
		~GraphicsEngine();
		template<typename Render>
		void RegisterInstruction(Render&& aRenderCall);
		void Draw();
	public:
		static GraphicsEngine* InitializeEngine(HWND anInstance);
	private:
		GraphicsEngine();
		void Init(HWND anInstance);
	private:

		IDrawer* myDrawer;
		ID3D11RenderTargetView* myBackBuffer;
		ID3D11DepthStencilView* myDepthBuffer;

		std::unordered_map<int, std::vector<std::shared_ptr<RenderCall>>> myInstructions;
	};


	template<typename Render>
	inline void GraphicsEngine::RegisterInstruction(Render&& aRenderCall)
	{
		std::shared_ptr<RenderCall> call = std::make_shared<DrawCall<Render>>(aRenderCall);
		myInstructions[call->myIID + call->myPSID + call->myVSID].push_back(call);
	}
}


