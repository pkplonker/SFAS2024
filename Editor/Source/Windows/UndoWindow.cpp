#include "UndoWindow.h"

#include "imgui.h"
#include "../ImGuiHelpers.h"
#include "../UndoManager.h"

UndoWindow::UndoWindow()
{
}

void UndoWindow::Draw()
{
    ImGui::Begin(UNDO_WINDOW.c_str());
    
    if (ImGuiHelpers::DrawIconButton(ICON_MD_UNDO))
    {
        UndoManager::Undo();
    }
    ImGui::SameLine();
    if (ImGuiHelpers::DrawIconButton(ICON_MD_REDO))
    {
        UndoManager::Redo();
    }
    ImGui::SameLine();
    if (ImGuiHelpers::DrawIconButton(ICON_MD_DELETE))
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
