#pragma once
#include <map>
#include <memory>
#include <xstring>

#include "imgui.h"

class Game;
class EditorWindow;
class DirectX11Graphics;

class ImGuiController
{
public:
	ImGuiController(DirectX11Graphics* dx11Graphics, Game* game);
	static void ImGuiPreFrame();
	void LoadScene() const;
	void LoadScene(std::string path) const;
	void DrawMenu();
	void DrawViewport();
	static void Save();
	void DrawWindows();
	void Draw();
	void ImGuiPostUpdate() const;
	void ShutDown();

private:
	DirectX11Graphics* dx11Graphics;
	Game* game;
	ImVec2 gameViewportSize;
	std::map<std::shared_ptr<EditorWindow>, bool> renderables;
};
