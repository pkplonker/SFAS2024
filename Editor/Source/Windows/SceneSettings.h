#pragma once
#include "EditorWindow.h"

class SceneSettings : public EditorWindow
{
public:
    void Draw() override;
    std::string GetName() override;
private:
    std::string name = "Scene Settings";
};
