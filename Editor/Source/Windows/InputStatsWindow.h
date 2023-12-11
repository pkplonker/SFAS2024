#pragma once
#include <string>

#include "EditorWindow.h"

class IInput;
const std::string INPUT_STAT = "Input Stats";

class InputStatsWindow : public EditorWindow
{
public:
    InputStatsWindow(IInput* input);
    void Draw() override;
    std::string GetName() override;
private:
    IInput* input;
    int count =0;
};
