﻿#pragma once
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <utility>

#include "Helpers.h"
#include "imgui.h"
#include "functional"
#include "ImGuiController.h"
#include "UndoManager.h"
#include "Engine/Math/Vector3.h"
#include "Engine/Math/Vector4.h"
#include "../External/IconsMaterialDesign.h"

class ImGuiHelpers
{
public:
    struct ButtonData
    {
        const char* label;
        std::function<void()> callback;
        bool icon = false;

        ButtonData(): label(nullptr), callback(nullptr)
        {
        }

        ButtonData(const char* label, std::function<void()> callback, bool icon = false): label(label),
            callback(callback), icon(icon)
        {
        }
    };

    template <typename StringType>
    static void WrappedText(const char* label, StringType content,
                            const std::vector<ButtonData>& buttons)
    {
        ImGui::AlignTextToFramePadding();

        ImGui::TextWrapped(label);
        ImGui::SameLine();

        for (const auto& button : buttons)
        {
            if (button.callback != nullptr)
            {
                std::ostringstream oss;
                oss << button.label << "##" << label;

                std::string buttonId = oss.str();
                if (!button.icon)
                {
                    if (ImGui::Button(buttonId.c_str()))
                    {
                        button.callback();
                    }
                }
                else
                {
                    if (DrawIconButton(button.label))
                    {
                        button.callback();
                    }
                }

                ImGui::SameLine();
            }
        }

        const float fullWidth = ImGui::GetWindowWidth();
        const float labelWidth = ImGui::CalcTextSize(label).x + ImGui::GetStyle().ItemSpacing.x;
        const float availableWidth = fullWidth - labelWidth - (ImGui::GetStyle().WindowPadding.x * 2);
        ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + availableWidth);
        ImGui::AlignTextToFramePadding();

        if constexpr (std::is_same<StringType, std::wstring>::value)
        {
            ImGui::TextWrapped(Helpers::WideStringToString(std::move(content)).c_str());
        }
        if constexpr (std::is_same<StringType, std::string>::value)
        {
            ImGui::TextWrapped(content.c_str());
        }

        ImGui::PopTextWrapPos();
    }

    template <typename StringType>
    static void WrappedText(const char* label, StringType content,
                            const char* singleButtonText, const std::function<void()>& singleButtonCallback,
                            bool icon = true)
    {
        auto data = ButtonData(singleButtonText, singleButtonCallback, icon);
        std::vector buttons = {{data}};
        WrappedText(label, content, buttons);
    }

    //https://github.com/ocornut/imgui/discussions/3862
    static bool ButtonCenteredOnLine(const char* label, bool icon = true, float alignment = 0.5f)
    {
        const ImGuiStyle& style = ImGui::GetStyle();

        const float size = ImGui::CalcTextSize(label).x + style.FramePadding.x * 2.0f;
        const float avail = ImGui::GetContentRegionAvail().x;

        const float off = (avail - size) * alignment;
        if (off > 0.0f)
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);

        return DrawIconButton(label, true);
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

    static inline float padding = 20;
    static inline float ypadding = 20;

    static bool DrawIconButton(const char* iconId, const bool large = true)
    {
        const float iconSize = large ? ImGuiController::GetLargeFontSize() : ImGuiController::GetSmallFontSize();
        const float oldPaddingY = ImGui::GetStyle().FramePadding.y;
        const float ypadding = large ? 20.0f : 10.0f;
        const float padding = large ? 8.0f : 6.0f;

        ImGui::PushFont(large ? ImGuiController::LargeIcons() : ImGuiController::SmallIcons());
        ImGui::GetStyle().FramePadding.y = ypadding;
        const bool pressed = ImGui::Button(iconId, ImVec2(iconSize + padding, iconSize + padding));
        ImGui::GetStyle().FramePadding.y = oldPaddingY;
        ImGui::PopFont();

        return pressed;
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
    static void UndoableDrag(
        std::function<T()> getValue,
        std::function<void(T)> setValue,
        const char* label,
        const std::string& actionDescription, float min = FLT_MIN, float max = FLT_MAX, float speed = 1)
    {
        T value = getValue();


        bool valueChanged = false;
        if constexpr (std::is_same<T, float>::value)
        {
            valueChanged = ImGui::DragFloat(label, reinterpret_cast<float*>(&value), speed, min, max);
        }
        else if constexpr (std::is_same<T, int>::value)
        {
            valueChanged = ImGui::DragInt(label, reinterpret_cast<int*>(&value), speed, min, max);
        }

        if (ImGui::IsItemActivated())
        {
            sliderStates[label] = static_cast<float>(getValue());
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
    static void UndoableColorEdit(
        std::function<T()> getValue,
        std::function<void(T)> setValue,
        const char* label,
        const std::string& actionDescription,
        ImGuiColorEditFlags flags = ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoBorder)
    {
        static std::map<std::string, T> colorEditStates;

        T value = getValue();

        std::string key = std::string(label) + "_" + std::to_string(reinterpret_cast<uintptr_t>(&value));
        auto originalVal = value;
        bool valueChanged = false;
        if constexpr (std::is_same<T, Vec3>::value)
        {
            valueChanged = ImGui::ColorEdit3(label, reinterpret_cast<float*>(&value), flags);
        }
        else if constexpr (std::is_same<T, Vec4>::value)
        {
            valueChanged = ImGui::ColorEdit4(label, reinterpret_cast<float*>(&value), flags);
        }

        if (ImGui::IsItemActivated())
        {
            colorEditStates[key] = originalVal;
        }

        if (valueChanged)
        {
            setValue(value);
        }

        if (ImGui::IsItemDeactivatedAfterEdit())
        {
            auto originalVal = colorEditStates[key];
            if (value != originalVal)
            {
                UndoManager::Execute(
                    Memento(
                        [setValue, value] { setValue(value); },
                        [setValue, originalVal] { setValue(originalVal); },
                        actionDescription));
            }
        }
    }


    template <typename T>
    static void UndoableMenuItemValue(
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

    static void UndoableButtonAction(
        const std::string& label,
        std::function<void()> doAction,
        std::function<void()> undoAction,
        const std::string& actionDescription)
    {
        if (ImGui::Button(label.c_str()))
        {
            UndoManager::Execute(
                Memento(
                    doAction,
                    undoAction,
                    actionDescription));
        }
    }

    static void UndoableCheckboxAction(
        const std::string& label,
        bool* variable,
        std::function<void()> doAction,
        std::function<void()> undoAction,
        const std::string& actionDescription)
    {
        if (ImGui::Checkbox(label.c_str(), variable))
        {
            UndoManager::Execute(
                Memento(
                    doAction,
                    undoAction,
                    actionDescription));
        }
    }

    static void DrawVector(const char* vectorName, Vec3& vector)
    {
        float labelWidth = ImGui::CalcTextSize("Rotation").x + 20.0f;
        float totalWidth = ImGui::GetContentRegionAvail().x - labelWidth;
        float fieldWidth = totalWidth / 3.0f - ImGui::GetStyle().ItemInnerSpacing.x;

        ImGui::Text(vectorName);
        ImGui::SameLine(labelWidth);

        ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(1, 0, 0, 1));
        ImGui::Text("X");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(fieldWidth - ImGui::CalcTextSize("X").x);
        UndoableDrag<float>(
            [&vector]() { return vector.X(); },
            [&vector](float x) { vector.X(x); },
            (std::string("##X") + vectorName).c_str(),
            "Change X Component"
        );
        ImGui::PopStyleColor();
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0, 1, 0, 1));
        ImGui::Text("Y");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(fieldWidth - ImGui::CalcTextSize("Y").x);
        UndoableDrag<float>(
            [&vector]() { return vector.Y(); },
            [&vector](float y) { vector.Y(y); },
            (std::string("##Y") + vectorName).c_str(),
            "Change Y Component"
        );
        ImGui::PopStyleColor();
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0, 0, 1, 1));
        ImGui::Text("Z");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(fieldWidth - ImGui::CalcTextSize("Z").x);
        UndoableDrag<float>(
            [&vector]() { return vector.Z(); },
            [&vector](float z) { vector.Z(z); },
            (std::string("##Z") + vectorName).c_str(),
            "Change Z Component"
        );
        ImGui::PopStyleColor();
    }

    static bool ButtonWithState(const char* str, ImVec2 buttonSize, bool state, bool activeOnTrue = true,
                                bool icon = true,
                                ImVec4 color = ImVec4(0.1372549086809158f, 0.1921568661928177f, 0.2627451121807098f,
                                                      1.0f))
    {
        bool style = state == activeOnTrue;
        bool result = false;
        if (style)
        {
            ImGui::PushStyleColor(ImGuiCol_Button, color);
        }
        if (icon)
        {
            if (DrawIconButton(str))
            {
                result = true;
            }
        }
        else
        {
            if (ImGui::Button(str, buttonSize))
            {
                result = true;
            }
        }

        if (style)
        {
            ImGui::PopStyleColor();
        }
        return result;
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
