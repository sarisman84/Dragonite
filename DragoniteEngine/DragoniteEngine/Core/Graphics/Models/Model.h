#pragma once
#include <memory>
#include "../DXIncludes.h"
#include "../../CU/Math/Vector4.h"
#include "../../CU/Math/Vector2.h"
#include "../../CU/Transform.h"

#include "../Textures/Texture.h"
namespace Dragonite
{
	struct Vertex
	{
		static Vertex Position(const float anX, const float anY, const float anZ, const float anW) noexcept
		{
			Vertex v;
			v.myPosition = Vector4f(anX, anY, anZ, anW);
			return v;
		}
		Vertex& UV(const float anX, const float anY) noexcept {
			myUV = Vector2f(anX, anY);
			return *this;
		}


		Vector4f myPosition;
		Vector2f myUV;
	};


	enum class PrimitiveType
	{
		Cube, Sphere, Screen
	};

	class Model
	{
		friend class GraphicsPipeline;
		friend class ModelFactory;
		friend class RenderID;
		DataBuffer
			myVertexBuffer,
			myIndexBuffer;
		size_t myIndexCount;
		const char* myName;



	};

	typedef std::shared_ptr<Model> ModelRef;

	class ModelInstance
	{
	public:
		friend class GraphicsPipeline;
		friend class ModelFactory;
		friend class RenderID;
		ModelRef myModel;
		Transform myTransform;
		TextureRef myTexture;
		const char* myModelName;
		const char* myTextureName;
		unsigned int myID;
	private:
		VertexShader
			myVertexShader;
		PixelShader
			myPixelShader;
		InputLayout
			myInputLayout;
	};
}
