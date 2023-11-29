#include "RenderStatWindow.h"

#include "IApplication.h"
#include "IGraphics.h"
#include "imgui.h"

void RenderStatWindow::Draw()
{
    ImGui::Begin("Draw Stats");

    auto stats = IApplication::GetGraphics()->currentStats;
    ImGui::Text("Draw Calls: %u", stats.drawCalls);
    ImGui::Text("Materials: %u", stats.materials);
    ImGui::Text("Shaders: %u", stats.shaders);
    ImGui::Separator();

    ImGui::Text("Tris: %u", stats.tris);
    ImGui::SameLine();
    ImGui::Text("Vertices: %u", stats.verts);
    ImGui::Separator();
    
    ImGui::Text("Screen Size: %u x %u", stats.width, stats.height);
    ImGui::Text("Viewport Size: %u x %u", stats.viewportWidth, stats.viewportHeight);
    ImGui::End();

}

std::string RenderStatWindow::GetName()
{
    return RENDER_STAT;
}
