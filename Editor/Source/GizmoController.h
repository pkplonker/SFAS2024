#pragma once
#include "ImGuiController.h"
#include "Engine/Math/Vector2.h"

struct Transform;
class EditorCamera;
class GameObject;
struct ImVec2;

enum GizmoOperation
{
    Translation,
    Rotation,
    Scale,
};

enum GizmoSpace
{
    World,
    Local,
};

class GizmoController
{
public:
    GizmoController(std::weak_ptr<EditorCamera> editorCamera,IInput* input);
    void ImGuiPreFrame();
    void Update(bool isInFocus, std::weak_ptr<GameObject> gameobject, ImVec2 size, ImVec2 position);
    DirectX::XMFLOAT4X4 ChangeMat(DirectX::XMMATRIX mat);
    void SetGizmoOpertion(GizmoOperation op);
    GizmoOperation GetGizmoOperation();
    void SetMode(GizmoSpace mode);
    GizmoSpace GetMode();

private:
    GizmoOperation operation = Translation;
    GizmoSpace space = World;
    std::weak_ptr<EditorCamera> editorCamera;
    float identityMatrix[16] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    IInput* input;
};
