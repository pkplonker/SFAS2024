#include "Inspector.h"

#include "Hierarchy.h"
#include "imgui.h"
#include "Engine/Implementation/GameObject.h"

Inspector::Inspector(std::shared_ptr<Hierarchy> hierarchy) : hierarchy(hierarchy),
                                                             factory(std::make_unique<ComponentDrawerFactory>())
{
}

void Inspector::Draw()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(12.0f, 12.0f));

    ImGui::Begin(INSPECTOR.c_str());

    if (const auto gameobject = hierarchy->GetSelectedObject().lock())
    {
        ImGui::Text(gameobject->Name.c_str());

        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
        {
            if (ImGui::BeginPopupContextItem("TransformContextMenu"))
            {
                if (ImGui::MenuItem("Reset Position"))
                {
                  gameobject->Transform()->Position = Vec3::Zero();
                }
                if (ImGui::MenuItem("Reset Rotation"))
                {
                    gameobject->Transform()->Rotation = Vec3::Zero();
                }
                if (ImGui::MenuItem("Reset Scale"))
                {
                    gameobject->Transform()->Scale = Vec3::One();
                }
                ImGui::EndPopup();
            }
            DrawVector("Position", gameobject->Transform()->Position);
            DrawVector("Rotation", gameobject->Transform()->Rotation);
            DrawVector("Scale", gameobject->Transform()->Scale);
        }


        for (auto component : gameobject->GetComponents())
        {
            auto drawer = factory->CreateDrawer(component);
            if (drawer != nullptr)
            {
                drawer->Draw();
            }
        }
    }
    else
    {
        ImGui::Text("Select an object to inspect.");
    }
    ImGui::PopStyleVar(1);
    ImGui::End();
}


void Inspector::DrawVector(const char* vectorName, Vec3& vector)
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
