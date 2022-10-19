#pragma once
#include "DirectX11/DXInterface.h"
#include "Models/Material.h"
#include "Core/CU/CommonData.h"
#include "Core/CU/Math/Matrix4x4.hpp"


#include <unordered_map>
#include <vector>
#include <memory>
#include "Textures/Texture.h"
#include <functional>


namespace Dragonite
{
	class CameraInterface;
	class Runtime;
	class ModelFactory;
	class TextureFactory;
	class ModelInstance;
	class BaseRenderer;
	class RenderFactory;
	class RenderTarget;
	class PollingStation;

	struct FrameBufferData
	{
		Matrix4x4f myWorldToClipMatrix;
	};

	struct ObjectBufferData
	{
		Matrix4x4f myModelToWorldMatrix;
	};

	struct RenderInstructions
	{
		unsigned int myShaderInstructionsID;

		DataBuffer myVertexBuffer;
		DataBuffer myIndexBuffer;
		Matrix4x4f myModelMatrix;
		TextureRef myTexture;
		unsigned int myIndexCount;
		unsigned int myID;
	};


	struct CameraInstructions
	{
		Matrix4x4f myViewMatrix;
		Matrix4x4f myProjectionMatrix;
	};

	struct ShaderInstructions
	{
		ShaderInstructions();
		Material myMaterial;
		InputLayout myInputLayout;
		VertexShader myVertexShader;
		PixelShader myPixelShader;
		int myIndex;


		bool operator==(const ShaderInstructions& aCpy);
		bool operator!=(const ShaderInstructions& aCpy);
	};




	struct Renderer
	{
		Renderer();
		virtual void OnRender(
			std::vector<RenderInstructions> someInstructions,
			CameraInstructions aCameraInstruction,
			ShaderInstructions someShaderInstructions,
			std::function<void(RenderInstructions)> anOnElementDrawCallback = nullptr) = 0;

	protected:
		DataBuffer myObjectBuffer, myFrameBuffer;
	};


	struct ForwardRenderer :public Renderer
	{
		ForwardRenderer();
		// Inherited via Renderer
		virtual void OnRender(
			std::vector<RenderInstructions> someInstructions,
			CameraInstructions aCameraInstruction,
			ShaderInstructions someShaderInstructions,
			std::function<void(RenderInstructions)> anOnElementDrawCallback = nullptr) override;

	};





	class GraphicalInterface
	{
		friend class Runtime;
	public:
		~GraphicalInterface();
		void SetActiveCameraAs(CameraInterface& aNewCamera);

		inline CameraInterface& GetActiveCamera() { return *myActiveCamera; }

		inline void RegisterRenderCall(const std::function<void()>& aCallback)
		{
			myExtraRenderCalls.push_back(aCallback);
		}

		inline PollingStation* GetPollingStation() { return myPollingStation; }
		inline void DrawToBackBuffer(const bool aNewVal) { myRenderToBackBufferFlag = aNewVal; }

		void AddRenderInstructions(const RenderInstructions& anInstruction);
		unsigned int AddShaderInstructions(const Material& aMaterial, VertexShader aVS, PixelShader aPS, InputLayout anInputLayout);
		inline std::vector<ShaderInstructions> GetShaderInstructions() const noexcept { return myShaderInstructions; }

		inline const bool ContainsShaderInstructions() { return !myShaderInstructions.empty(); }

		void DrawInstructions(
			ShaderInstructions aShaderInstruciton,
			std::function<void(RenderInstructions)> anOnElementDrawCallback = nullptr);
		void DrawInstructions(
			VertexShader aVS, PixelShader aPS, InputLayout anIL,
			std::function<void(RenderInstructions)> anOnElementDrawCallback = nullptr
		);

		void DrawInstructions(std::function<void(RenderInstructions)> anOnElementDrawCallback = nullptr);

	private:
		const bool Init(HWND anInstance, PollingStation* aPollingStation);
		void Render();

		bool myRenderToBackBufferFlag;
		CameraInterface* myActiveCamera;
		PollingStation* myPollingStation;

		Renderer* myPrimaryRenderer;
		std::vector<std::function<void()>> myExtraRenderCalls;
		std::vector<RenderInstructions> myRenderInstructions;

		std::vector<ShaderInstructions> myShaderInstructions;

	};





}

