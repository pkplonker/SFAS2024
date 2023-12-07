#pragma once
#include "ICamera.h"
#include "IUpdateable.h"

class IInput;
class PerspectiveCamera;

class EditorCamera : public ICamera, public std::enable_shared_from_this<EditorCamera>, public IUpdateable
{
public:
    EditorCamera(IInput* input);
    void SetActiveCamera();
    std::shared_ptr<Transform3D> GetTransform() override;
    void SetTransform(std::shared_ptr<Transform3D> transform) override;
    DirectX::XMMATRIX GetProjectionMatrix() override;
    DirectX::XMMATRIX GetViewMatrix() override;
    DirectX::XMMATRIX GetViewProjectionMatrix() override;
    void SetHeight(float getHeight) override;
    void SetWidth(float getHeight) override;
    void SetFov(int fov);
    void Update() override;
    std::shared_ptr<PerspectiveCamera> camera;

private:
    std::shared_ptr<Transform3D> transform;
    IInput* input;
};
