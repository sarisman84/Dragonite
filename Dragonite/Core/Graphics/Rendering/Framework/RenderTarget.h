#pragma once

#include <wrl/client.h>

#include "Component.h"
using Microsoft::WRL::ComPtr;

struct ID3D11DeviceContext;
struct ID3D11ShaderResourceView;
struct ID3D11RenderTargetView;

struct ModelInstance;

namespace Dragonite
{
	class GraphicsEngine;
}

class RenderTarget : public Component
{
public:
	RenderTarget();
	~RenderTarget() override;
	// Inherited via Component
	void OnAwake() override;
	void OnUpdate(float aDeltaTime) override;

	inline ComPtr<ID3D11RenderTargetView>& RenderTexture() { return myRenderTarget; }
	void Render(Dragonite::GraphicsEngine* aGraphicsEngine);

private:
	void InitializeTexture();


	ComPtr<ID3D11ShaderResourceView> myTextureResource;
	ComPtr<ID3D11RenderTargetView> myRenderTarget;
	std::shared_ptr<ModelInstance> myInstance;
};



