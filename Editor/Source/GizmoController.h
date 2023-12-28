#pragma once
#include "ImGuiController.h"
#include "Engine/Math/Vector2.h"
struct Transform;
class EditorCamera;
class GameObject;

struct ImVec2;

class GizmoController
{
public:
    GizmoController(std::weak_ptr<EditorCamera> editorCamera);
    void ImGuiPreFrame(ImVec2 size, ImVec2 position);
    void Update(std::weak_ptr<GameObject> gameobject, ImVec2 size, ImVec2 position);
    DirectX::XMFLOAT4X4 ChangeMat(DirectX::XMMATRIX mat);

private:
    std::weak_ptr<EditorCamera> editorCamera;
};
