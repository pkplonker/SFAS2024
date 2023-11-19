#pragma once
#include "IimGuiDraw.h"
#include "imgui.h"
#include "Math/Vector3.h"
#include <string>

struct Transform3D : IimGuiDraw
{
    Transform3D() : Transform3D(Vec3::Zero(), Vec3::Zero(), Vec3::One())
    {
    }

    Transform3D(Vec3 position, Vec3 rotation, Vec3 scale)
    {
        this->Position = position;
        this->Rotation = rotation;
        this->Scale = scale;
    }

    Transform3D(Vec3 position, Vec3 rotation) : Transform3D(position, rotation, Vec3::One())
    {
    }

    Transform3D(Vec3 position) : Transform3D(position, Vec3::Zero(), Vec3::One())
    {
    }

    Vec3 Position = {0};
    Vec3 Rotation = {0};
    Vec3 Scale = {1};

    void ImGuiDraw() override
    {
        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
        {
            if (ImGui::BeginPopupContextItem("TransformContextMenu"))
            {
                if (ImGui::MenuItem("Reset Position"))
                {
                    Position = Vec3::Zero();
                }
                if (ImGui::MenuItem("Reset Rotation"))
                {
                    Rotation = Vec3::Zero();
                }
                if (ImGui::MenuItem("Reset Scale"))
                {
                    Scale = Vec3::One();
                }
                ImGui::EndPopup();
            }
            DrawVector("Position", Position);
            DrawVector("Rotation", Rotation);
            DrawVector("Scale", Scale);
        }
    }


    void DrawVector(const char* vectorName, Vec3& vector)
    {
        float labelWidth = ImGui::CalcTextSize("Rotation").x + 12.0f;
        float totalWidth = ImGui::GetContentRegionAvail().x - labelWidth;
        float fieldWidth = totalWidth / 3.0f - ImGui::GetStyle().ItemInnerSpacing.x;

        ImGui::Text(vectorName);
        ImGui::SameLine(labelWidth);

        ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(1, 0, 0, 1));
        ImGui::Text("X");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(fieldWidth - ImGui::CalcTextSize("X").x);
        ImGui::DragFloat((std::string("##X") + vectorName).c_str(), &vector.X(), 0.1f, -FLT_MAX, FLT_MAX, "%.3f");
        ImGui::PopStyleColor();
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0, 1, 0, 1));
        ImGui::Text("Y");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(fieldWidth - ImGui::CalcTextSize("Y").x);
        ImGui::DragFloat((std::string("##Y") + vectorName).c_str(), &vector.Y(), 0.1f, -FLT_MAX, FLT_MAX, "%.3f");
        ImGui::PopStyleColor();
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0, 0, 1, 1));
        ImGui::Text("Z");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(fieldWidth - ImGui::CalcTextSize("Z").x);
        ImGui::DragFloat((std::string("##Z") + vectorName).c_str(), &vector.Z(), 0.1f, -FLT_MAX, FLT_MAX, "%.3f");
        ImGui::PopStyleColor();
    }
};
