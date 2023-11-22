#pragma once
#include "ImGuiWindow.h"

const std::string FPS_COUNTER = "Fps Counter";

class ImGuiFPSCounter : public EditorWindow
{
public:
	ImGuiFPSCounter();

	void Draw() override;
	std::string GetName() override { return FPS_COUNTER; }
};
