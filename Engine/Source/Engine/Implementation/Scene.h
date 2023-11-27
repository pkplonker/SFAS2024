#pragma once
#include <memory>
#include <set>

#include "Engine/Implementation/GameObject.h"
#include "Engine/IRenderable.h"

class ImGuiController;

class Scene : public IUpdateable
{
public:
	Scene(IGraphics* graphics);
	~Scene() override;
	void SetScene(IGraphics* graphics){this->graphics = graphics;}
	void AddObject(std::shared_ptr<GameObject> object);
	void RemoveObject(std::shared_ptr<GameObject> object);
	void AddRenderable(std::shared_ptr<IRenderable> object) const;
	void RemoveRenderable(std::shared_ptr<IRenderable> object) const;
	void Update() override;
	void AddRenderable(const std::shared_ptr<GameObject>& object) const;
	void SetActiveCamera(const std::shared_ptr<ICamera>& camera);
	void DrawScene();
	void DrawInspector();
	void DrawCamera();
	std::set<std::shared_ptr<GameObject>>& GetObjects() const;

private:
	std::unique_ptr<std::set<std::shared_ptr<GameObject>>> objects = std::make_unique<std::set<std::shared_ptr<
		GameObject>>>();
	std::unique_ptr<std::set<std::shared_ptr<IRenderable>>> renderables = std::make_unique<std::set<std::shared_ptr<
		IRenderable>>>();
	IGraphics* graphics={};
	std::shared_ptr<ICamera> camera={};
	std::weak_ptr<GameObject> selectedObject={};
};
