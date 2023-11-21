#pragma once
#include "imgui.h"
class DirectX11Graphics;

class ImGuiController
{
public:
	ImGuiController(DirectX11Graphics* dx11Graphics);
	void ImGuiPreFrame();
	void ImGuiDrawViewport();
	void ImGuiPostUpdate();
	void ShutDown();

private:
	DirectX11Graphics* dx11Graphics;
	ImVec2 gameViewportSize;
};
