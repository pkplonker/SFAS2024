#include "IGraphics.h"

IGraphics::IGraphics() : Renderables()
{
}

IGraphics::~IGraphics()
{
	Renderables.clear();
	Textures.clear();
}