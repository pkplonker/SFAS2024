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
private:
	
	std::shared_ptr<GameObject> camera;

};
