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

    //https://github.com/ocornut/imgui/discussions/3862
    static bool ButtonCenteredOnLine(const char* label, float alignment = 0.5f)
    {
        ImGuiStyle& style = ImGui::GetStyle();

        float size = ImGui::CalcTextSize(label).x + style.FramePadding.x * 2.0f;
        float avail = ImGui::GetContentRegionAvail().x;

        float off = (avail - size) * alignment;
        if (off > 0.0f)
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);

        return ImGui::Button(label);
    }

    static void SpacedSeperator(int firstGap = 1, int secondGap = 1)
    {
        for (auto i = 0; i < firstGap; i++)
        {
            ImGui::NewLine();
        }
        ImGui::Separator();
        for (auto i = 0; i < secondGap; i++)
        {
            ImGui::NewLine();
        }
    }
};
