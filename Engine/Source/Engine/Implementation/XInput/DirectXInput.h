#pragma once

#include <memory>

#include "Engine/IInput.h"
#include <windows.h>
#include <XInput.h>

#include "Keyboard.h"
#include "Mouse.h"


enum MouseButtons
{
    Left = 0,
    Right = 1,
    Middle = 2
};

enum MouseStates
{
    Pressed = 0,
    Released = 1,
    Up = 3,
    Held = 4,
};

class DirectXInput : public IInput
{
public:
    DirectXInput(HWND window);
    virtual ~DirectXInput();
    void ProcessMouse(UINT msg, WPARAM uint, LPARAM lparam) const;
    void ProcessKeyboard(UINT msg, WPARAM uint, LPARAM lparam) const ;
    void HandleController();
    void HandleMouse();
    void HandleKeyboard();
    void Update() override;

private:
    float CalculateTriggerValue(BYTE rawValue, float threshold = XINPUT_GAMEPAD_TRIGGER_THRESHOLD, float max = 255.0f);
    float CalculateAxisValue(SHORT rawValue, float deadzone, float left = -32768.0f, float right = 32767.0f);
    std::unique_ptr<DirectX::Mouse> mouse;
    std::unique_ptr<DirectX::Keyboard> keyboard;
    DirectX::Mouse::ButtonStateTracker tracker;

    XINPUT_STATE State;
};
