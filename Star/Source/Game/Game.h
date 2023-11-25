#pragma once
#include <memory>
#include "Engine/IApplication.h"
#include "Engine/ResourceManager.h"

class GameObject;
class Scene;

class Game : public IApplication
{
public:

	Game(IGraphics* Graphics, IInput* InputIn);
	virtual ~Game();

	virtual bool IsValid();
	virtual bool Load();
	virtual void Update();
	virtual void Cleanup();
	virtual void PostGraphics();
	std::weak_ptr<Scene> GetScene();

private:
	
	std::shared_ptr<Scene> scene;
	std::unique_ptr<ResourceManager> resourceManager;
	std::shared_ptr<GameObject> camera;
};
