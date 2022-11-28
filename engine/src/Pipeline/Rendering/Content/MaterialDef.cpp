#include "MaterialDef.h"
#include <d3d11.h>

Dragonite::Texture::Texture()
{
	myResourceView = nullptr;
}

Dragonite::Texture::~Texture()
{
	if (myResourceView)
		myResourceView->Release();
	myResourceView = nullptr;
}

Dragonite::Texture::Texture(ID3D11ShaderResourceView* aResourceView)
{
	myResourceView = aResourceView;
}

void Dragonite::Texture::Bind(ID3D11DeviceContext* aContext)
{
	if (!myResourceView || aContext) return;


	aContext->PSSetShaderResources(mySlot, 1, &myResourceView);
}