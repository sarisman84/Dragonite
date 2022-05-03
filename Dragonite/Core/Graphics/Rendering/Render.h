#pragma once
#include <wrl/client.h>
#include "../CommonUtilities.h"
#include <string>

using Microsoft::WRL::ComPtr;
using namespace CommonUtilities::Math;

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11Buffer;
struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11InputLayout;




namespace Engine
{
	namespace Graphics
	{
		struct Vertex
		{
			Vector4<float> myPosition;
			Vector4<float> myColor;
		};



		struct MeshInfo
		{
			Vertex myVerticies[100];
			unsigned int myVerticiesAmm;

			unsigned int myIndices[100];
			unsigned int myIndicesAmm;
		};


		class Render
		{
			friend class GraphicsEngine;
		public:
			Render(ID3D11Device* aDevice, ID3D11DeviceContext* aDeviceContext);
			~Render();

		private:
			virtual MeshInfo OnInit() = 0;
			bool Initialize();
			void Draw();
			bool TryCreateVertexBuffer(long& someResult, Vertex* someVertices, unsigned int aSize);
			bool TryCreateIndexBuffer(long& someResult, unsigned int* someIndicies, unsigned int anIndeciesAmm);

			bool TryLoadVertexShader(long& someResult, const std::string& aPath, std::string& someExtraData);
			bool TryLoadPixelShader(long& someResult, const std::string& aPath);

			ComPtr<ID3D11Buffer> myVertexBuffer;
			ComPtr<ID3D11Buffer> myIndexBuffer;
			ComPtr<ID3D11VertexShader> myVertexShader;
			ComPtr<ID3D11PixelShader> myPixelShader;
			ComPtr<ID3D11InputLayout> myInputLayout;

			ID3D11Device* myDevice;
			ID3D11DeviceContext* myContext;
			unsigned int myIndicesAmm;

		

		protected:
			Matrix4x4<float> myObjectMatrix;
			const char* myVertexShaderPath;
			const char* myPixelShaderPath;

		};
	}
}



