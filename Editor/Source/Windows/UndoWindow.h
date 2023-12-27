#pragma once
#include <string>

#include "EditorWindow.h"
const std::string UNDO_WINDOW = "Undo";

class UndoWindow : public EditorWindow
{
public:
    UndoWindow();

    void Draw() override;
    std::string GetName() override { return UNDO_WINDOW; }
};
