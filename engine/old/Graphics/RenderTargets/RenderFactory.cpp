#include "Core/Graphics/GraphicsAPI.h"
#include "Core/Graphics/RenderTargets/RenderTarget.h"
#include "RenderFactory.h"

#pragma comment(lib, "d3d11.lib")
bool Dragonite::RenderFactory::Init(GraphicalInterface* aPipeline)
{
    if (!aPipeline) return false;
    myPipeline = aPipeline;


    return true;
}

void Dragonite::RenderFactory::RegisterTarget(RenderTarget& aTarget)
{
  
}


//TODO: Refactor the API (Maybe RenderPasses can help)