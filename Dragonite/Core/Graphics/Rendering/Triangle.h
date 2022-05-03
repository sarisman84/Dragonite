#pragma once
#include "Render.h"

using namespace Engine::Graphics;

class Triangle : public Render
{
public:
	Triangle(ID3D11Device* aDevice, ID3D11DeviceContext* aDeviceContext, Vector3<float> aPosition, Vector3<float> aSize);
	// Inherited via Render
	virtual MeshInfo OnInit() override;


};

