#include "TimeWindow.h"

#include <chrono>

#include "imgui.h"
#include "EngineTime.h"

TimeWindow::TimeWindow()
{
}

void TimeWindow::Draw()
{
	ImGui::Begin("EngineTime");
	
	ImGui::Text(("Current time:" + EngineTime::GetCurrentTime()).c_str());
	ImGui::Text(("Running time:" + EngineTime::GetElapsedFormatted()).c_str());
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,
		ImGui::GetIO().Framerate);
	
	ImGui::End();
}