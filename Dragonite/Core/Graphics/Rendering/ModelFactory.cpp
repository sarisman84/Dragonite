#include "ModelFactory.h"
#include <d3d11.h>
#include "Graphics/GraphicsEngine.h"
using namespace Engine::Graphics;

MeshData GetUnitCube()
{
	MeshData data;


	data.myVertecies.emplace_back(Vertex{ Math::Vector4f(-1, -1, 1, 1), Math::Vector4f::one }); //0
	data.myVertecies.emplace_back(Vertex{ Math::Vector4f(-1,  1, 1, 1), Math::Vector4f::one }); //1
	data.myVertecies.emplace_back(Vertex{ Math::Vector4f(1,  1, 1, 1), Math::Vector4f::one }); //2
	data.myVertecies.emplace_back(Vertex{ Math::Vector4f(1, -1, 1, 1), Math::Vector4f::one }); //3
	data.myVertecies.emplace_back(Vertex{ Math::Vector4f(1, -1,-1, 1), Math::Vector4f::one }); //4
	data.myVertecies.emplace_back(Vertex{ Math::Vector4f(-1,  1,-1, 1), Math::Vector4f::one }); //5
	data.myVertecies.emplace_back(Vertex{ Math::Vector4f(1,  1,-1, 1), Math::Vector4f::one }); //6
	data.myVertecies.emplace_back(Vertex{ Math::Vector4f(-1, -1,-1, 1), Math::Vector4f::one }); //7


	data.myIndicies = {
		//Front vv
				0,1,2,
				3,0,2,

		//Botton vv
				0,3,4,
				4,7,0,

		//Left   vv
				7,1,0,
				1,7,6,

		//Right  vv
				2,4,3,
				5,4,2,

		//Top    vv
				2,5,1,
				1,5,6,
	};

	return data;
}

ModelFactory::ModelFactory()
{
	ModelPtr cube = std::make_shared<Model>();
	MeshData data = GetUnitCube();



	InitializeBuffers(data);



	cube->myMesh.push_back(data);



	myTypes["UnitCube"] = cube;


}

ModelFactory::~ModelFactory() = default;

void ModelFactory::CreateModel(std::string aType, Model aModel)
{
	myTypes[aType] = std::make_shared<Model>(aModel);
}

ModelInstance ModelFactory::GetModel(std::string aType)
{
	ModelInstance ins;
	ins.myModel = myTypes[aType];

	std::string* vertexData;
	myEngine->InitializeShader(ShaderType::Vertex, ins.myVertexShader, ins.myVertexShaderPath, vertexData);
	myEngine->InitializeShader(ShaderType::Pixel, ins.myPixelShader, ins.myPixelShaderPath);

	myEngine->InitializeLayout(ins.myInputLayout, vertexData);



	return ins;
}

void Engine::Graphics::ModelFactory::InitializeBuffers(MeshData & someData)
{
	myEngine->InitializeBuffer(BufferType::Vertex, someData.myVertexBuffer, someData.myVertecies, someData.myVertecies.size());
	myEngine->InitializeBuffer(BufferType::Index, someData.myIndexBuffer, someData.myIndicies, someData.myIndicies.size());
}

