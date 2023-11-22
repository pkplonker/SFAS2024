#include "ImGuiFPSCounter.h"
#include "imgui.h"

ImGuiFPSCounter::ImGuiFPSCounter()
{
}

void ImGuiFPSCounter::Draw()
{
	ImGui::Begin("FPS");
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,
		ImGui::GetIO().Framerate);
	ImGui::End();
}