#include "UndoWindow.h"

#include "imgui.h"
#include "../UndoManager.h"

UndoWindow::UndoWindow()
{
}

void UndoWindow::Draw()
{
    ImGui::Begin(UNDO_WINDOW.c_str());
    
    if (ImGui::Button("Undo"))
    {
        UndoManager::Undo();
    }
    ImGui::SameLine();
    if (ImGui::Button("Redo"))
    {
        UndoManager::Redo();
    }
    ImGui::SameLine();
    if (ImGui::Button("Clear"))
    {
        UndoManager::Clear();
    }
    if (ImGui::BeginChild("Scrolling", ImVec2(0, 0), true,
                          ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_AlwaysVerticalScrollbar))
    {
        for (auto command : UndoManager::GetCommandDescriptions())
        {
            ImGui::Text(command.c_str());
        }
    }
    ImGui::EndChild();
    ImGui::End();
}
