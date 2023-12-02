#pragma once

#include "ImGuiWindow.h"

const std::string TIME_WINDOW = "Time";

class TimeWindow : public EditorWindow
{
public:
	TimeWindow();

	void Draw() override;
	std::string GetName() override { return TIME_WINDOW; }
};
