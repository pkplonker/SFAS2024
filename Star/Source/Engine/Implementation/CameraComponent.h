﻿#pragma once
#include <DirectXMath.h>

#include "Engine/ICamera.h"
#include "Engine/IComponent.h"

class CameraComponent : public IComponent, public ICamera
{
public:
    CameraComponent(std::weak_ptr<GameObject> object);
    CameraComponent(std::weak_ptr<GameObject> object, std::shared_ptr<ICamera> camera);
    void SetCamera(std::shared_ptr<ICamera> cam);
    std::shared_ptr<ICamera> GetCamera();
    DirectX::XMMATRIX GetProjectionMatrix() override;
    DirectX::XMMATRIX GetViewMatrix() override;
    DirectX::XMMATRIX GetViewProjectionMatrix() override;

private:
    std::shared_ptr<ICamera> camera;
};

