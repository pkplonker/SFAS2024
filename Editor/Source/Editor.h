#pragma once

#include <memory>

#include "Game.h"
#include "imgui.h"
#include "Engine/IApplication.h"
#include "Engine/Implementation/Scene.h"

class DirectX11Graphics;
class ResourceManager;
class GameObject;
class IGraphics;
class ITexture;
class IShader;
class IRenderable;
struct HWND__;
typedef struct HWND__* HWND;
typedef void* HANDLE;

class Editor : public IApplication
{
public:

	Editor(IGraphics* Graphics, IInput* InputIn, HWND hwnd);
	virtual ~Editor();

	virtual bool IsValid();
	virtual bool Load();
	virtual void Update();
	virtual void Cleanup();
	virtual void PostGraphics();

private:

	std::shared_ptr<GameObject> camera;
	DirectX11Graphics* dx11Graphics;
	Game* game;
	HWND hwnd;
	ImVec2 gameViewportSize;
	std::unique_ptr<ImGuiController> imguiController;
};
