#pragma once
#include <string>
#include <utility>

#include "Helpers.h"
#include "imgui.h"
#include "functional"

class ImGuiHelpers
{
public:
    template <typename StringType>
    static void WrappedText(std::string label, StringType content,
                            const std::vector<std::pair<std::string, std::function<void()>>>& buttons)
    {
        ImGui::Text(label.c_str());
        ImGui::SameLine();

        for (const auto& [buttonText, callback] : buttons)
        {
            if (callback != nullptr)
            {
                std::ostringstream oss;
                oss << buttonText << "##" << label;

                std::string buttonId = oss.str();
                if (ImGui::Button(buttonId.c_str()))
                {
                    callback();
                }
                ImGui::SameLine();
            }
        }

        float fullWidth = ImGui::GetWindowWidth();
        float labelWidth = ImGui::CalcTextSize(label.c_str()).x + ImGui::GetStyle().ItemSpacing.x;
        float availableWidth = fullWidth - labelWidth - (ImGui::GetStyle().WindowPadding.x * 2);
        ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + availableWidth);

        if constexpr (std::is_same<StringType, std::wstring>::value)
        {
            ImGui::TextUnformatted(Helpers::WideStringToString(std::move(content)).c_str());
        }
        else
        {
            ImGui::TextUnformatted(content.c_str());
        }

        ImGui::PopTextWrapPos();
    }

    template <typename StringType>
    static void WrappedText(std::string label, StringType content,
                            const std::string& singleButtonText, const std::function<void()>& singleButtonCallback)
    {
        std::vector<std::pair<std::string, std::function<void()>>> buttons = {
            {singleButtonText, singleButtonCallback}
        };

        WrappedText(label, content, buttons);
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
