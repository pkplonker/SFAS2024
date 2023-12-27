#pragma once
#include <iostream>
#include <string>
#include <utility>

#include "Helpers.h"
#include "imgui.h"
#include "functional"
#include "UndoManager.h"
#include "Engine/Implementation/Logging/Debug.h"
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

    template <typename T>
    static void UndoableSlider(
        std::function<T()> getValue,
        std::function<void(T)> setValue,
        const char* label,
        T minValue,
        T maxValue,
        const std::string& actionDescription)
    {
        T value = getValue();


        bool valueChanged = false;
        if constexpr (std::is_same<T, float>::value)
        {
            valueChanged = ImGui::SliderFloat(label, reinterpret_cast<float*>(&value), minValue, maxValue);
        }
        else if constexpr (std::is_same<T, int>::value)
        {
            valueChanged = ImGui::SliderInt(label, reinterpret_cast<int*>(&value), minValue, maxValue);
        }

        if (ImGui::IsItemActivated())
        {
            sliderStates[label] = static_cast<float>(getValue());
            std::cout << "setting inital value: " << static_cast<float>(getValue()) << std::endl;
        }

        if (valueChanged)
        {
            setValue(value);
        }

        if (ImGui::IsItemDeactivatedAfterEdit())
        {
            auto originalVal = static_cast<T>(sliderStates[label]);
            if (value != originalVal)
            {
                UndoManager::Execute(
                    Memento(
                        [setValue, value] { setValue(value); },
                        [setValue,originalVal] { setValue(static_cast<T>(originalVal)); },
                        actionDescription));
            }
        }
    }

    template <typename T>
    static void UndoableMenuItem(
        const std::string& label,
        std::function<T()> getter,
        std::function<void(const T&)> setter,
        const T& newState,
        const std::string& actionDescription)
    {
        T originalState = getter();

        if (ImGui::MenuItem(label.c_str()))
        {
            setter(newState);

            UndoManager::Execute(
                Memento(
                    [setter, newState] { setter(newState); },
                    [setter, originalState] { setter(originalState); },
                    actionDescription));
        }
    }

    static void UndoableMenuItemAction(
        const std::string& label,
        std::function<void()> doAction,
        std::function<void()> undoAction,
        const std::string& actionDescription)
    {
        if (ImGui::MenuItem(label.c_str()))
        {
            UndoManager::Execute(
                Memento(
                    doAction,
                    undoAction,
                    actionDescription));
        }
    }

private:
    bool IsClicked()
    {
        return ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left);
    }

    static inline bool isActivated = false;
    static inline bool wasActive = false;
    static inline std::unordered_map<std::string, float> sliderStates = {};
};
