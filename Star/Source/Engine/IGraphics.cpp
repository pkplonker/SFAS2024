#include "IGraphics.h"
#include "ITexture.h"
#include "IShader.h"
#include "IRenderable.h"


IGraphics::IGraphics() : Renderables()
{
}

IGraphics::~IGraphics()
{
    Renderables.clear();

    for (auto texture = Textures.begin(); texture != Textures.end(); ++texture)
    {
        delete (*texture);
    }

    Textures.clear();
}
