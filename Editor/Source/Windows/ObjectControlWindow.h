#pragma once
#include "EditorWindow.h"

const std::string NAME = "ObjectControl";

class ObjectControlWindow: public EditorWindow
{
public:
    void Draw() override;
    std::string GetName() override;
};
