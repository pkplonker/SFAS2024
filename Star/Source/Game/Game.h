#pragma once

#include <memory>

#include "Engine/IApplication.h"
#include "Engine/Implementation/Scene.h"

class ResourceManager;
class GameObject;
class IGraphics;
class ITexture;
class IShader;
class IRenderable;

enum RingLayer { Outer, Middle, Inner, NumRings };
enum GameState { Setup, Playing, Test, NumModes };
static const unsigned int NumberOfRings = static_cast<int>(NumRings);

class Game : public IApplication
{
public:

	Game(IGraphics* Graphics, IInput* InputIn, ImGuiController* ImGui);
	virtual ~Game();

	virtual bool IsValid();
	virtual bool Load();
	virtual void Update();
	virtual void Cleanup();

private:

	void SetupEachRing();
	void UpdateRingSelection();
	void UpdateSelectedRingRotation();
	void UpdateRingTestSelection();
	void TestRingSolution();

	std::shared_ptr<GameObject> Rings[NumberOfRings];
	std::shared_ptr<GameObject> Arrow;
	RingLayer SelectedRing;
	GameState State;
	std::shared_ptr<Scene> scene;
	std::unique_ptr<ResourceManager> resourceManager;
	std::shared_ptr<GameObject> camera;
};
