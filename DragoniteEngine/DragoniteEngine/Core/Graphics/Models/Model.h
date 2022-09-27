#pragma once
#include <memory>
#include "../DXIncludes.h"

namespace Dragonite
{
	enum class PrimitiveType
	{
		Cube, Sphere
	};

	class Model
	{
		friend class ModelFactory;
		DataBuffer
			myVertexBuffer,
			myIndexBuffer;
		VertexShader
			myVertexShader;
		PixelShader
			myPixelShader;
		InputLayout
			myInputLayout;
	};

	typedef std::shared_ptr<Model> ModelPtr;

	struct ModelInstance
	{
		ModelPtr myModel;
	};
}
