#pragma once
#include "ImGuiController.h"
#include "Engine/Math/Vector2.h"
struct Transform;
class EditorCamera;
class GameObject;
class GizmoController
{
public:
    GizmoController(std::weak_ptr<EditorCamera> editorCamera);
    void ImGuiPreFrame(Vec2 size);
    void Update(std::weak_ptr<GameObject> transform);
private:
    std::weak_ptr<EditorCamera> editorCamera;
};
