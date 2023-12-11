#pragma once
#include <map>
#include <memory>
#include <xstring>

#include "IInput.h"
#include "imgui.h"
#include "IResizeHandler.h"
#include "Logging/BufferSink.h"

class SettingsWindow;
class Game;
class EditorWindow;
class DirectX11Graphics;

class ImGuiController : public IResizeHandler
{
public:
	ImGuiController(DirectX11Graphics* dx11Graphics, Game* game, IInput* input);
~ImGuiController() override;
	static void ImGuiPreFrame();
	void LoadScene() const;
	void LoadScene(std::string path) const;
	void DrawMenu();
	void DrawViewport();
	void AddWindow(const std::shared_ptr<EditorWindow>& shared);
	static void Save();
	void DrawWindows();
	void Draw();
	void ImGuiPostUpdate() const;
	void ShutDown();
	void Resize(int width, int height) override;

private:
	DirectX11Graphics* dx11Graphics;
	Game* game;
	ImVec2 gameViewportSize;
	std::map<std::shared_ptr<EditorWindow>, bool> renderables;
	BufferSink* bufferSink;
	IInput* input;
	bool openSettings;
	SettingsWindow* settingsWindow;
};
