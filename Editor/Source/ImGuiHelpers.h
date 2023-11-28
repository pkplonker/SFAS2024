
#pragma once
#include <string>
#include <utility>

#include "Helpers.h"
#include "imgui.h"

class ImGuiHelpers
{
public:
    static void WrappedText(std::string label, std::string content)
    {
        ImGui::Text(label.c_str());
        ImGui::SameLine();
        float fullWidth = ImGui::GetWindowWidth();
        float labelWidth = ImGui::CalcTextSize(label.c_str()).x + ImGui::GetStyle().ItemSpacing.x;
        float availableWidth = fullWidth - labelWidth -(ImGui::GetStyle().WindowPadding.x * 2);
        ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + availableWidth);
        ImGui::TextUnformatted(content.c_str());
        ImGui::PopTextWrapPos();
    }
    static void WrappedText(std::string label, std::wstring content)
    {
        ImGui::Text(label.c_str());
        ImGui::SameLine();
        float fullWidth = ImGui::GetWindowWidth();
        float labelWidth = ImGui::CalcTextSize(label.c_str()).x + ImGui::GetStyle().ItemSpacing.x;
        float availableWidth = fullWidth - labelWidth -(ImGui::GetStyle().WindowPadding.x * 2);
        ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + availableWidth);
        ImGui::TextUnformatted(Helpers::WideStringToString(std::move(content)).c_str());
        ImGui::PopTextWrapPos();
    }
};
