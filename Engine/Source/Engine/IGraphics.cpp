#include "IGraphics.h"

IGraphics::IGraphics() : Renderables()
{
}

IGraphics::~IGraphics()
{
    Renderables.clear();
    Textures.clear();
}

void IGraphics::AddRenderStrategy(std::shared_ptr<IRenderStrategy> strategy)
{
    postRenderStrategies.emplace_back(strategy);
}

void IGraphics::RemoveRenderStrategy(std::shared_ptr<IRenderStrategy> strategy)
{
    if (auto it = std::ranges::find(postRenderStrategies, strategy); it != postRenderStrategies.end())
    {
        postRenderStrategies.erase(it);
    }
}
