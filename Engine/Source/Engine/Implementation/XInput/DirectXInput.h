#pragma once

#include <memory>

#include "Engine/IInput.h"
#include <windows.h>
#include <XInput.h>

#include "Keyboard.h"
#include "Mouse.h"

struct Vec2;

class DirectXInput : public IInput
{
public:
    DirectXInput(HWND window);
    virtual ~DirectXInput();
    void ProcessMouse(UINT msg, WPARAM uint, LPARAM lparam) const;
    void ProcessKeyboard(UINT msg, WPARAM uint, LPARAM lparam) const;
    void HandleController();
    void HandleMouse();
    void HandleKeyboard();
    void Update() override;
    bool IsLeftPressed() const override;
    bool IsLeftHeld() const override;
    bool IsLeftReleased() const override;

    bool IsRightPressed() const override;
    bool IsRightHeld() const override;
    bool IsRightReleased() const override;

    bool IsMiddlePressed() const override;
    bool IsMiddleHeld() const override;
    bool IsMiddleReleased() const override;

    int GetMouseX() const override;
    int GetMouseY() const override;
    int GetDeltaX() const override;
    int GetDeltaY() const override;
    Vec2 GetMousePosition() override;
    Vec2 GetMouseDelta() override;
    int GetMouseScrollDelta() override;
private:
    float CalculateTriggerValue(BYTE rawValue, float threshold = XINPUT_GAMEPAD_TRIGGER_THRESHOLD, float max = 255.0f);
    float CalculateAxisValue(SHORT rawValue, float deadzone, float left = -32768.0f, float right = 32767.0f);


private:
    std::unique_ptr<DirectX::Mouse> mouse{};
    std::unique_ptr<DirectX::Keyboard> keyboard{};
    DirectX::Mouse::ButtonStateTracker tracker;
    XINPUT_STATE State;
    bool leftPressed = false;
    bool leftHeld = false;
    bool leftReleased = false;

    bool rightPressed = false;
    bool rightHeld = false;
    bool rightReleased = false;

    bool middlePressed = false;
    bool middleHeld = false;
    bool middleReleased = false;

    int mouseX = 0;
    int mouseY = 0;
    int lastX = 0;
    int lastY = 0;
    int deltaX = 0;
    int deltaY = 0;
    int scrollDelta=0;
    int lastScrollValue=0;
    inline static bool mouseDataUpdated;
    inline static bool keyboardDataUpdated;
};
