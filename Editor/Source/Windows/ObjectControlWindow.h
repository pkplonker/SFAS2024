#pragma once
#include <memory>

#include "EditorWindow.h"

class GizmoController;
const std::string NAME = "ObjectControl";

class ObjectControlWindow : public EditorWindow
{
public:
    ObjectControlWindow(std::shared_ptr<GizmoController> gizmoController);
    void Draw() override;
    std::string GetName() override;

private:
    std::shared_ptr<GizmoController> gizmoController;
};
