#pragma once

#include <memory>

#include "Game.h"
#include "Engine/IApplication.h"
#include "Engine/Implementation/Scene.h"

class DirectX11Graphics;
class ResourceManager;
class GameObject;
class IGraphics;
class ITexture;
class IShader;
class IRenderable;

class Editor : public IApplication
{
public:

	Editor(IGraphics* Graphics, IInput* InputIn);
	virtual ~Editor();

	virtual bool IsValid();
	virtual bool Load();
	virtual void Update();
	virtual void Cleanup();
	virtual void PostGraphics();
private:

	std::shared_ptr<GameObject> camera;
	DirectX11Graphics* dx11Graphics;
};
