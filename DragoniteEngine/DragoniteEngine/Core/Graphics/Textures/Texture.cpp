#include "Texture.h"
#include <d3d11.h>

Dragonite::Texture::Texture()
{
}

Dragonite::Texture::~Texture()
{
}

bool Dragonite::Texture::Init(Device& aDevice, unsigned char* someRgbaPixels, Vector2i aResolution)
{
    DXTexture2D texture;

    D3D11_TEXTURE2D_DESC desc = {};

    desc.Width = aResolution.x;
    desc.Height = aResolution.y;
    desc.MipLevels = 1;
    desc.ArraySize = 1;

    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Usage = D3D11_USAGE_IMMUTABLE;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = 0;


    D3D11_SUBRESOURCE_DATA subResrcDesc = {};
    subResrcDesc.pSysMem = (void*)someRgbaPixels;
    subResrcDesc.SysMemPitch = aResolution.x * 4;
    subResrcDesc.SysMemSlicePitch = aResolution.x * aResolution.y * 4;

    if (FAILED(aDevice->CreateTexture2D(&desc, &subResrcDesc, &texture)))
    {
        return false;
    }

    if (FAILED(aDevice->CreateShaderResourceView(texture.Get(), NULL, &myResourceView)))
    {
        return false;
    }


    return true;
}

void Dragonite::Texture::Bind(DeviceContext& someContext, int aSlot)
{
    someContext->PSSetShaderResources(aSlot, 1, myResourceView.GetAddressOf());
}
