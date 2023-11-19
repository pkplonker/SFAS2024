#include "IApplication.h"
#include "IGraphics.h"
#include "IInput.h"

IApplication::IApplication(IGraphics* GraphicsIn, IInput* InputIn, ImGuiController* ImGui) : Graphics(GraphicsIn), Input(InputIn), ImGui(ImGui)
{
}

IApplication::~IApplication()
{
}