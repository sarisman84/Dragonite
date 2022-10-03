#pragma once
#include <map>
#include <vector>
#include <memory>
#include "Core/Utilities/EnumMap.h"
namespace Dragonite
{
	class ModelInstance;
	class RenderTarget;
	class RenderFactory
	{
	public:
		RenderFactory() = default;

		template<typename Key>
		void CreateRenderTarget(const Key aKey, RenderTarget* aRenderTarget);

		template<typename Key>
		std::unique_ptr<RenderTarget>& GetRenderTarget(const Key aKey);

	private:
		EnumMap<std::unique_ptr<RenderTarget>> myRenderTargets;
	};
	template<typename Key>
	inline void RenderFactory::CreateRenderTarget(const Key aKey, RenderTarget* aRenderTarget)
	{
		myRenderTargets[aKey] = std::unique_ptr<RenderTarget>(aRenderTarget);
	}
	template<typename Key>
	inline std::unique_ptr<RenderTarget>& RenderFactory::GetRenderTarget(const Key aKey)
	{
		return myRenderTargets[aKey];
	}
}


