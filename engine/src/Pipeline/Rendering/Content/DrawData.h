#pragma once
#include "DrawContent.h"
#include "entt/single_include/entt/entt.hpp"

#include <cstdint>

namespace Dragonite
{
	struct DrawData : public IDrawData
	{
		friend class GraphicsEngine;
		//Shader ids
		uint32_t myILID;
		uint32_t myVSID;
		uint32_t myPSID;

		//Vertex ids4
		
		uint32_t myVSBufferID;
		uint32_t myIndiciesID;

		//Entity ref
		entt::entity myTargetEntity;

	};
}