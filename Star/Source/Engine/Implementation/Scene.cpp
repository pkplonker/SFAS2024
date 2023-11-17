#include "Scene.h"

#include "imgui.h"
#include "MeshRenderable.h"
#include "SpriteRenderable.h"
#include "Engine/ICamera.h"
#include "Engine/ImGuiController.h"
class SpriteRenderable;
const std::string SCENE = "Scene Hierarchy";

Scene::Scene(IGraphics* graphics, ImGuiController* ImGui)
{
    objects = std::make_unique<std::set<std::shared_ptr<
        GameObject>>>();
    renderables = std::make_unique<std::set<std::shared_ptr<
        IRenderable>>>();
    this->ImGui = ImGui;
    this->graphics = graphics;
    ImGui->RegisterWindow(this, SCENE);
}

void Scene::AddObject(std::shared_ptr<GameObject> object)
{
    objects->emplace(object);
    AddRenderable(object);
}

void Scene::AddRenderable(std::shared_ptr<IRenderable> object) const
{
    renderables->emplace(object);
}

void Scene::Update()
{
    for (const std::shared_ptr<GameObject>& object : *objects)
    {
        object->Update();
    }
}

void Scene::AddRenderable(std::shared_ptr<GameObject> object) const
{
    std::shared_ptr<IRenderableComponent> renderableComponent = object->GetComponent<IRenderableComponent>();
    if (renderableComponent != nullptr)
    {
        renderables->emplace(renderableComponent->GetRenderable());
    }
}

void Scene::SetActiveCamera(std::shared_ptr<ICamera> camera)
{
    this->camera = camera;
    camera->SetWidth(graphics->GetWidth());
    camera->SetHeight(graphics->GetHeight());
    graphics->SetActiveCamera(camera);
}

void Scene::Render(std::string window)
{
    if (window == SCENE)
    {
        ImGui::Begin("Scene");
        ImGui::Text("Test test test");
        ImGui::End();
    }
}
