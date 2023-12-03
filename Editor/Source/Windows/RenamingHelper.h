#pragma once
#include <memory>

#include "GameObject.h"
#include "imgui.h"

class RenamingHelper
{
public:
    RenamingHelper() : showRenamePopup(false)
    {
        memset(renameBuffer, 0, sizeof(renameBuffer));
    }

    void RequestRename(const std::shared_ptr<GameObject>& object)
    {
        objectToRename = object;
        strncpy_s(renameBuffer, object->Name.c_str(), RENAME_BUFFER_SIZE);
        renameBuffer[RENAME_BUFFER_SIZE - 1] = '\0';
        showRenamePopup = true;
    }

    void DrawRenamePopup()
    {
        if (showRenamePopup)
        {
            ImGui::SetNextWindowPos(ImGui::GetMousePos());
            ImGui::OpenPopup("Rename Object");
            showRenamePopup = false;
        }

        if (ImGui::BeginPopupModal("Rename Object", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::InputText("##rename", renameBuffer, RENAME_BUFFER_SIZE);
            if (ImGui::Button("OK"))
            {
                if (auto obj = objectToRename.lock())
                {
                    obj->Name = renameBuffer;
                }
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel"))
            {
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
    }

private:
    std::weak_ptr<GameObject> objectToRename;
    char renameBuffer[256]{};
    bool showRenamePopup;
    static constexpr size_t RENAME_BUFFER_SIZE = 256;
};
