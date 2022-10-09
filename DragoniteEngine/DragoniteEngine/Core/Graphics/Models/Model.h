#pragma once
#include <memory>
#include "Core/Graphics/DirectX11/DXIncludes.h"
#include "Core/CU/Math/Vector4.h"
#include "Core/CU/Math/Vector2.h"
#include "Core/CU/Transform.h"

#include "Core/Graphics/Textures/Texture.h"
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
	public:
		friend class GraphicalInterface;
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
		friend class GraphicalInterface;
		friend class ModelFactory;
		friend class RenderID;
		ModelRef myModel;
		Transform myTransform;
		TextureRef myTexture;
		const char* myModelName;
		const char* myTextureName;
		unsigned int myShaderInstructionsID;
	};
}
