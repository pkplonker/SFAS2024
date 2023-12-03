#pragma once
#include <memory>

#include "EditorWindow.h"

class EditorCamera;
const std::string CAMERA_WINDOW = "Editor Camera";
class EditorCameraWindow : public EditorWindow
{
public:
    EditorCameraWindow(std::shared_ptr<EditorCamera> camera);
    void Draw() override;
    std::string GetName() override;

private:
    std::shared_ptr<EditorCamera> camera;
};
