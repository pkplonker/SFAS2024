#include "IApplication.h"
#include "IGraphics.h"
#include "IInput.h"

IApplication::IApplication(IGraphics* GraphicsIn, IInput* InputIn)
{
    IApplication::Graphics = GraphicsIn;
    IApplication::Input = InputIn;
}

IApplication::~IApplication()
{
    Graphics = nullptr;
    Input = nullptr;
}
