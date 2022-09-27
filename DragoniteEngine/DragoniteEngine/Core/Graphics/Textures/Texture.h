#pragma once
#include "../DXIncludes.h"
#include "../../CU/Math/Vector2.h"

namespace Dragonite
{
	class Texture
	{
	public:
		Texture();
		~Texture();

		bool Init(Device& aDevice, unsigned char* someRgbaPixels, Vector2i aResolution);
		void Bind(DeviceContext& someContext, int aSlot);
	private:
		ShaderResourceV myResourceView;
	};
}


