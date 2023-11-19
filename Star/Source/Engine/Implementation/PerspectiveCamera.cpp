#include "PerspectiveCamera.h"

#include "imgui.h"

PerspectiveCamera::PerspectiveCamera(float width, float height, float near, float far, float FOV): near(near),
    far(far), width(width), height(height), FOV(FOV), view(DirectX::XMMatrixIdentity()),
    projection(DirectX::XMMatrixIdentity()), vpMatrix(DirectX::XMMatrixIdentity())
{
}

DirectX::XMMATRIX PerspectiveCamera::GetProjectionMatrix()
{
    projection = DirectX::XMMatrixPerspectiveFovLH(FOV, width / height, near, far);
    return projection;
}

DirectX::XMMATRIX PerspectiveCamera::GetViewMatrix()
{
    view = DirectX::XMMatrixLookAtLH(GetEyePosition(), GetFocusPoint(), GetUpDirection());
    return view;
}

DirectX::XMMATRIX PerspectiveCamera::GetViewProjectionMatrix()
{
    GetProjectionMatrix();
    GetViewMatrix();
    vpMatrix = XMMatrixMultiply(view, projection);
    return vpMatrix;
}


DirectX::XMVECTOR PerspectiveCamera::GetFocusPoint()
{
    return DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);;
}

DirectX::XMVECTOR PerspectiveCamera::GetEyePosition()
{
    return DirectX::XMVectorSet(0.0f, 0.0f, -10.0f, 1.0f);;
}

DirectX::XMVECTOR PerspectiveCamera::GetUpDirection()
{
    return DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
}

void PerspectiveCamera::ImGuiDraw()
{
    if (ImGui::TreeNodeEx("Perspective Camera Settings", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (ImGui::BeginPopupContextItem("PerspectiveCameraContext"))
        {
            if (ImGui::MenuItem("Reset Near"))
            {
                near = DEFAULT_NEAR;
            }
            if (ImGui::MenuItem("Reset Far"))
            {
                far = DEFAULT_FAR;
            }
            if (ImGui::MenuItem("Reset FOV"))
            {
                FOV = DEFAULT_FOV;
            }
            ImGui::EndPopup();
        }
        ImGui::SliderFloat("Field of View", &FOV, 0.01f,3.141f);
        ImGui::DragFloat("Width", &width,1,1);
        ImGui::DragFloat("Height", &height,1,1);
        ImGui::DragFloat("Near Plane", &near,1,0.1f);
        ImGui::DragFloat("Far Plane", &far,1,0.1f);
        ImGui::TreePop();
    }
}


void PerspectiveCamera::SetHeight(int height)
{
    this->height = static_cast<float>(height);
}

void PerspectiveCamera::SetWidth(int width)
{
    this->width = static_cast<float>(width);
}

void PerspectiveCamera::SetNear(float near)
{
    this->near = near;
}

void PerspectiveCamera::SetFar(float far)
{
    this->far = far;
}

void PerspectiveCamera::SetFOV(float fov)
{
    this->FOV = fov;
}
