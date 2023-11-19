#include "Scene.h"

#include "CameraComponent.h"
#include "imgui.h"
#include "MeshRenderable.h"
#include "SpriteRenderable.h"
#include "Engine/ICamera.h"
#include "Engine/ImGuiController.h"
class SpriteRenderable;
const std::string SCENE = "Scene Hierarchy";
const std::string INSPECTOR = "Inspector";
const std::string CAMERA = "Camera";

Scene::Scene(IGraphics* graphics, ImGuiController* ImGui)
{
    objects = std::make_unique<std::set<std::shared_ptr<
        GameObject>>>();
    renderables = std::make_unique<std::set<std::shared_ptr<
        IRenderable>>>();
    this->ImGui = ImGui;
    this->graphics = graphics;
    ImGui->RegisterWindow(this, SCENE);
    ImGui->RegisterWindow(this, INSPECTOR);
    ImGui->RegisterWindow(this, CAMERA);
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

void Scene::AddRenderable(const std::shared_ptr<GameObject>& object) const
{
    std::shared_ptr<IRenderableComponent> renderableComponent = object->GetComponent<IRenderableComponent>();
    if (renderableComponent != nullptr)
    {
        renderables->emplace(renderableComponent->GetRenderable());
    }
}

void Scene::SetActiveCamera(const std::shared_ptr<ICamera>& camera)
{
    this->camera = camera;
    camera->SetWidth(graphics->GetWidth());
    camera->SetHeight(graphics->GetHeight());
    graphics->SetActiveCamera(camera);
}

void Scene::DrawScene()
{
    ImGui::Begin(SCENE.c_str());

    for (const auto& object : *objects)
    {
        std::string label = object->Name;
        bool is_selected = (selectedObject.lock() == object);
        if (ImGui::Selectable(label.c_str(), is_selected))
        {
            selectedObject = object;
        }
    }

    ImGui::End();
    return;
}

void Scene::DrawInspector()
{
    ImGui::Begin(INSPECTOR.c_str());

    auto gameobject = selectedObject.lock();
    if (gameobject)
    {
        ImGui::Text(gameobject->Name.c_str());
        gameobject->ImGuiDraw();
    }
    else
    {
        ImGui::Text("Select an object to inspect.");
    }

    ImGui::End();
}

void Scene::DrawCamera()
{
    ImGui::Begin(CAMERA.c_str());
    if (camera)
    {
        auto cameraComponent = std::dynamic_pointer_cast<CameraComponent>(camera);
        if (cameraComponent)
        {
            cameraComponent->ImGuiDraw();
            
        }
        
        
    
    }
    else
    {
        ImGui::Text("No camera attached");
    }
    ImGui::End();
}

void Scene::ImGuiRender(std::string window)
{
    if (window == SCENE)
    {
        DrawScene();
        return;
    }
    if (window == INSPECTOR)
    {
        DrawInspector();
        return;
    }
    if (window == CAMERA)
    {
        DrawCamera();
        return;
    }
}
