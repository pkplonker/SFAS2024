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

    float GetXRotSpeed() const { return xRotSpeed; }
    float GetYRotSpeed() const { return yRotSpeed; }
    float GetXMoveSpeed() const { return xMoveSpeed; }
    float GetYMoveSpeed() const { return yMoveSpeed; }
    float GetScrollSpeed() const { return scrollSpeed; }

    void SetXRotSpeed(float speed) { xRotSpeed = speed; }
    void SetYRotSpeed(float speed) { yRotSpeed = speed; }
    void SetXMoveSpeed(float speed) { xMoveSpeed = speed; }
    void SetYMoveSpeed(float speed) { yMoveSpeed = speed; }
    void SetScrollSpeed(float speed) { scrollSpeed = speed; }

private:
    std::shared_ptr<Transform3D> transform;
    IInput* input;
    float xRotSpeed = 1.0f;
    float yRotSpeed = 1.0f;
    float xMoveSpeed = 1.0f;
    float yMoveSpeed = 1.0f;
    float scrollSpeed = 1.0f;
};
