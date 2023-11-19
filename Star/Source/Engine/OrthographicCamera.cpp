#include "OrthographicCamera.h"

#include "imgui.h"
#include "Implementation/DirectX11/DirectX11Billboard.h"

OrthographicCamera::OrthographicCamera(float width, float height, float nearZ, float farZ) : width(width),
    height(height), nearZ(nearZ), farZ(farZ)
{
}


DirectX::XMMATRIX OrthographicCamera::GetProjectionMatrix()
{
    projection = DirectX::XMMatrixOrthographicLH(width, height, nearZ, farZ);
    return projection;
}

DirectX::XMMATRIX OrthographicCamera::GetViewMatrix()
{
    view = DirectX::XMMatrixIdentity();
    return view;
}


DirectX::XMMATRIX OrthographicCamera::GetViewProjectionMatrix()
{
    vpMatrix = XMMatrixMultiply(GetViewMatrix(), GetProjectionMatrix());
    return vpMatrix;
}

void OrthographicCamera::SetHeight(int height)
{
    this->height = static_cast<float>(height);
}

void OrthographicCamera::SetWidth(int width)
{
    this->width = static_cast<float>(width);
}

void OrthographicCamera::ImGuiDraw()
{
    if (ImGui::TreeNodeEx("Orthographic Camera Settings",ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (ImGui::BeginPopupContextItem("OrthographicCameraContext"))
        {
            if (ImGui::MenuItem("Reset Near"))
            {
                nearZ = DEFAULT_NEAR;
            }
            if (ImGui::MenuItem("Reset Far"))
            {
                farZ = DEFAULT_FAR;
            }
            ImGui::EndPopup();
        }
        ImGui::DragFloat("Width", &width);
        ImGui::DragFloat("Height", &height);
        ImGui::DragFloat("Near Plane", &nearZ);
        ImGui::DragFloat("Far Plane", &farZ);

        ImGui::TreePop();
    }
}
