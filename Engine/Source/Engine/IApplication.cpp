#include "IApplication.h"

#include "ComponentRegistry.h"
#include "IGraphics.h"
#include "IInput.h"

IApplication::IApplication(IGraphics* GraphicsIn, IInput* InputIn)
{
    IApplication::Graphics = GraphicsIn;
    IApplication::Input = InputIn;
    ComponentRegistry::RegisterComponents(); // This could be handled by the components directly.
}

IApplication::~IApplication()
{
    Graphics = nullptr;
    Input = nullptr;
}
