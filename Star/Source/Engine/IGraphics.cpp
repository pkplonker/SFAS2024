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
    

    Textures.clear();
}

