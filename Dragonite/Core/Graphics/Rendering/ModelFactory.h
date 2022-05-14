#pragma once
#include <vector>
#include "Utilities/Math/Vector4.h"
#include <wrl/client.h>
#include <typeinfo>
#include <map>
#include <string>
#include "CommonComponents.h"
#include "MeshInfo.h"
using Microsoft::WRL::ComPtr;

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
		class GraphicsEngine;
		class ModelFactory
		{
		public:
			ModelFactory();
			~ModelFactory();
			inline void FetchGraphicsEngine(GraphicsEngine* anEngine) { myEngine = anEngine; }
			void CreateModel(std::string aType, Model aModel);
			ModelInstance GetModel(std::string aType);

		private:
			void InitializeBuffers(MeshData& someData);


			std::map<std::string, ModelPtr> myTypes;
			GraphicsEngine* myEngine;
		};
	}
}




