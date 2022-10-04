#include "RenderUtils.h"
#include <fstream>

#include <d3d11.h>

HRESULT Dragonite::RenderUtils::CreateVSInstance(Device aDevice, const char* aPath, VertexShader& aShader, std::string& someExtraData)
{
	typedef std::istreambuf_iterator<char> ShaderIterator;
	std::string path = "resources/shaders/" + std::string(aPath) + "_VS.cso";
	std::ifstream file(path, std::ios::binary);

	if (!file || file.fail())
		return E_ACCESSDENIED;

	bool hasFailed = file.is_open();
	someExtraData = { std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>() };
	file.close();
	hasFailed = file.is_open();

	HRESULT r = aDevice->CreateVertexShader(someExtraData.data(), someExtraData.size(), nullptr, &aShader);
	return r;
}

HRESULT Dragonite::RenderUtils::CreatePSInstance(Device aDevice, const char* aPath, PixelShader& aShader)
{
	typedef std::istreambuf_iterator<char> ShaderIterator;
	std::string data;

	std::ifstream file;
	std::string path = "resources/shaders/" + std::string(aPath) + "_PS.cso";
	file.open(path, std::ios::binary);
	data = { ShaderIterator(file), ShaderIterator() };

	return aDevice->CreatePixelShader(data.data(), data.size(), nullptr, &aShader);
}
