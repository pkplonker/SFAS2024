#pragma once
#include <DirectXMath.h>

#include "Engine/ICamera.h"
#include "Engine/IComponent.h"

struct Transform3D;

class CameraComponent : public IComponent, public ICamera, public std::enable_shared_from_this<CameraComponent>

{
public:
    CameraComponent(std::weak_ptr<GameObject> object);
    CameraComponent(std::weak_ptr<GameObject> object, std::shared_ptr<ICamera> camera);
    void SetCamera(std::shared_ptr<ICamera> cam);
    std::shared_ptr<ICamera> GetCamera();
    DirectX::XMMATRIX GetProjectionMatrix() override;
    DirectX::XMMATRIX GetViewMatrix() override;
    DirectX::XMMATRIX GetViewProjectionMatrix() override;
    void SetHeight(float height) override;
    void SetWidth(float width) override;
    std::shared_ptr<Transform3D> GetTransform() override;
    void SetTransform(std::shared_ptr<Transform3D> transform) override { camera->SetTransform(transform); }

private:
    std::shared_ptr<ICamera> camera;
};
