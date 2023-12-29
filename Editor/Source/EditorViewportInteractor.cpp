#include "EditorViewportInteractor.h"

#include <iostream>

#include "IInput.h"
#include "ImGuiController.h"
#include "SceneManager.h"
#include "Implementation/Scene.h"

EditorViewportInteractor::EditorViewportInteractor(IInput* input, ImGuiController* controller) : controller(controller),
    input(input)
{
}

void EditorViewportInteractor::Update(std::weak_ptr<ICamera> cameraComponent)
{
    if (!controller->IsViewportInFocus() || !input->IsLeftPressed())
    {
        return;
    }
    auto viewportPosition = controller->GetViewportPosition();
    auto viewportSize = controller->GetViewportSize();
    DirectX::XMVECTOR rayOrigin, rayDirection;

    std::cout << "mousePos:" << input->GetMousePosition() << std::endl;

    ScreenPointToRay(input->GetMousePosition(), viewportSize.x, viewportSize.y, cameraComponent, rayOrigin,
                     rayDirection, viewportPosition);
    if (const auto scene = SceneManager::GetScene().lock())
    {
        float distanceToIntersection = 0.0f;
        bool found = false;
        for (auto [guid, obj] : scene->GetObjects())
        {
            if (obj->GetAABB().Intersects(rayOrigin, rayDirection, distanceToIntersection))
            {
                //std::cout << "selected! " << obj->Name << "Distance: " << distanceToIntersection <<std::endl;
                controller->SetSelectedObject(obj);
                found = true;
                break;
            }
        }
        if(!found)
        {
            controller->SetSelectedObject(nullptr);
        }
    }
}

void EditorViewportInteractor::ScreenPointToRay(Vec2 mousePos, int viewportWidth, int viewportHeight,
                                                std::weak_ptr<ICamera> cameraComponent, DirectX::XMVECTOR& rayOrigin,
                                                DirectX::XMVECTOR& rayDirection, ImVec2 viewportPosition)
{
    if (const auto cam = cameraComponent.lock())
    {
        float adjustedMouseX = static_cast<float>(mousePos.X()) - viewportPosition.x;
        float adjustedMouseY = static_cast<float>(mousePos.Y()) - viewportPosition.y;

        float x = (2.0f * adjustedMouseX) / static_cast<float>(viewportWidth) - 1.0f;
        float y = 1.0f - (2.0f * adjustedMouseY) / static_cast<float>(viewportHeight);
        DirectX::XMVECTOR ndcPos = DirectX::XMVectorSet(x, y, 1.0f, 1.0f);
        DirectX::XMFLOAT4 ndcPosFloat4;
        DirectX::XMStoreFloat4(&ndcPosFloat4, ndcPos);

        // std::cout << "NDC Position: X=" << ndcPosFloat4.x
        //     << ", Y=" << ndcPosFloat4.y << std::endl;
        DirectX::XMMATRIX invProjMatrix = DirectX::XMMatrixInverse(nullptr, cam->GetProjectionMatrix());
        DirectX::XMVECTOR viewSpacePos = DirectX::XMVector3TransformCoord(ndcPos, invProjMatrix);

        DirectX::XMMATRIX invViewMatrix = DirectX::XMMatrixInverse(nullptr, cam->GetViewMatrix());
        DirectX::XMVECTOR worldSpacePos = DirectX::XMVector3TransformCoord(viewSpacePos, invViewMatrix);

        rayOrigin = DirectX::XMVector3TransformCoord(DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f), invViewMatrix);
        rayDirection = DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(worldSpacePos, rayOrigin));
        // std::cout << "RayOrigin: " << rayOrigin << std::endl;
        // std::cout << "RayDir: " << rayDirection << std::endl;
    }
}
