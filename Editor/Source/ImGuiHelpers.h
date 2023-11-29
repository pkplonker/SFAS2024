#pragma once
#include <string>
#include <utility>

#include "Helpers.h"
#include "imgui.h"
#include "functional"

class ImGuiHelpers
{
public:
    static void WrappedText(std::string label, std::string content, std::function<void()> callback = nullptr,
                            std::string buttonText = "...")
    {
        ImGui::Text(label.c_str());
        ImGui::SameLine();
        if (callback != nullptr)
        {
            std::string buttonId = buttonText + "##" + label + content;

            if (ImGui::Button(buttonId.c_str()))
            {
                callback();
            }
            ImGui::SameLine();

        }

        float fullWidth = ImGui::GetWindowWidth();
        float labelWidth = ImGui::CalcTextSize(label.c_str()).x + ImGui::GetStyle().ItemSpacing.x;
        float availableWidth = fullWidth - labelWidth - (ImGui::GetStyle().WindowPadding.x * 2);
        ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + availableWidth);
        ImGui::TextUnformatted(content.c_str());
        ImGui::PopTextWrapPos();
    }

    static void WrappedText(std::string label, std::wstring content, std::function<void()> callback = nullptr,
                            std::string buttonText = "...")
    {
        ImGui::Text(label.c_str());
        ImGui::SameLine();
        if (callback != nullptr)
        {
            std::string buttonId = buttonText + "##" + label + Helpers::WideStringToString(content);

            if (ImGui::Button(buttonId.c_str()))
            {
                callback();
            }
        }

        ImGui::SameLine();
        float fullWidth = ImGui::GetWindowWidth();
        float labelWidth = ImGui::CalcTextSize(label.c_str()).x + ImGui::GetStyle().ItemSpacing.x;
        float availableWidth = fullWidth - labelWidth - (ImGui::GetStyle().WindowPadding.x * 2);
        ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + availableWidth);
        ImGui::TextUnformatted(Helpers::WideStringToString(std::move(content)).c_str());
        ImGui::PopTextWrapPos();
    }
};
