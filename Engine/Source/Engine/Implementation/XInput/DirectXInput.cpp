#include "DirectXInput.h"
#include <math.h>

#include "Keyboard.h"
#include "Mouse.h"

DirectXInput::DirectXInput(HWND window) : IInput(), State()
{
    keyboard = std::make_unique<DirectX::Keyboard>();
    mouse = std::make_unique<DirectX::Mouse>();
    mouse->SetWindow(window);
}

DirectXInput::~DirectXInput()
{
}

void DirectXInput::ProcessMouse(UINT msg, WPARAM wParam, LPARAM lParam) const
{
    DirectX::Mouse::ProcessMessage(msg, wParam, lParam);
}

void DirectXInput::ProcessKeyboard(UINT msg, WPARAM wParam, LPARAM lParam) const
{
    DirectX::Keyboard::ProcessMessage(msg, wParam, lParam);
}

void DirectXInput::HandleController()
{
    unsigned int result = XInputGetState(0, &State);

    // Buttons
    CurrentState[static_cast<unsigned int>(InputAction::ButtonLeft)] = State.Gamepad.wButtons & XINPUT_GAMEPAD_X
                                                                           ? 1.0f
                                                                           : 0.0f;
    CurrentState[static_cast<unsigned int>(InputAction::ButtonRight)] = State.Gamepad.wButtons & XINPUT_GAMEPAD_B
                                                                            ? 1.0f
                                                                            : 0.0f;
    CurrentState[static_cast<unsigned int>(InputAction::ButtonTop)] = State.Gamepad.wButtons & XINPUT_GAMEPAD_Y
                                                                          ? 1.0f
                                                                          : 0.0f;
    CurrentState[static_cast<unsigned int>(InputAction::ButtonBottom)] = State.Gamepad.wButtons & XINPUT_GAMEPAD_A
                                                                             ? 1.0f
                                                                             : 0.0f;
    CurrentState[static_cast<unsigned int>(InputAction::DirectionPadLeft)] =
        State.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT ? 1.0f : 0.0f;
    CurrentState[static_cast<unsigned int>(InputAction::DirectionPadRight)] =
        State.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT ? 1.0f : 0.0f;
    CurrentState[static_cast<unsigned int>(InputAction::DirectionPadTop)] =
        State.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP ? 1.0f : 0.0f;
    CurrentState[static_cast<unsigned int>(InputAction::DirectionPadBottom)] =
        State.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN ? 1.0f : 0.0f;
    CurrentState[static_cast<unsigned int>(InputAction::LeftStickPress)] =
        State.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB ? 1.0f : 0.0f;
    CurrentState[static_cast<unsigned int>(InputAction::RightStickPress)] =
        State.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB ? 1.0f : 0.0f;
    CurrentState[static_cast<unsigned int>(InputAction::ShoulderButtonLeft)] =
        State.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER ? 1.0f : 0.0f;
    CurrentState[static_cast<unsigned int>(InputAction::ShoulderButtonRight)] =
        State.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER ? 1.0f : 0.0f;
    CurrentState[static_cast<unsigned int>(InputAction::SpecialLeft)] = State.Gamepad.wButtons & XINPUT_GAMEPAD_BACK
                                                                            ? 1.0f
                                                                            : 0.0f;
    CurrentState[static_cast<unsigned int>(InputAction::SpecialRight)] = State.Gamepad.wButtons & XINPUT_GAMEPAD_START
                                                                             ? 1.0f
                                                                             : 0.0f;

    // Triggers
    CurrentState[static_cast<unsigned int>(InputAction::TriggerLeft)] = CalculateTriggerValue(
        State.Gamepad.bLeftTrigger);
    CurrentState[static_cast<unsigned int>(InputAction::TriggerRight)] = CalculateTriggerValue(
        State.Gamepad.bRightTrigger);

    // Stick Axis
    CurrentState[static_cast<unsigned int>(InputAction::LeftStickXAxis)] = CalculateAxisValue(
        State.Gamepad.sThumbLX, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
    CurrentState[static_cast<unsigned int>(InputAction::LeftStickYAxis)] = CalculateAxisValue(
        State.Gamepad.sThumbLY, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
    CurrentState[static_cast<unsigned int>(InputAction::RightStickXAxis)] = CalculateAxisValue(
        State.Gamepad.sThumbRX, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
    CurrentState[static_cast<unsigned int>(InputAction::RightStickYAxis)] = CalculateAxisValue(
        State.Gamepad.sThumbRY, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
}

void DirectXInput::HandleMouse()
{
    auto state = mouse->GetState();
    tracker.Update(state);


    CurrentState[static_cast<int>(InputAction::MouseLeft)] = tracker.leftButton ==
                                                             DirectX::Mouse::ButtonStateTracker::PRESSED || tracker.
                                                             leftButton == DirectX::Mouse::ButtonStateTracker::HELD
                                                                 ? 1.0f
                                                                 : 0.0f;
    CurrentState[static_cast<int>(InputAction::MouseRight)] = tracker.rightButton ==
                                                              DirectX::Mouse::ButtonStateTracker::PRESSED || tracker.
                                                              leftButton == DirectX::Mouse::ButtonStateTracker::HELD
                                                                  ? 1.0f
                                                                  : 0.0f;
    CurrentState[static_cast<int>(InputAction::MouseMiddle)] = tracker.middleButton ==
                                                               DirectX::Mouse::ButtonStateTracker::PRESSED || tracker.
                                                               leftButton == DirectX::Mouse::ButtonStateTracker::HELD
                                                                   ? 1.0f
                                                                   : 0.0f;
}

void DirectXInput::HandleKeyboard()
{
}

void DirectXInput::Update()
{
    HandleController();
    HandleMouse();
    HandleKeyboard();
}

float DirectXInput::CalculateTriggerValue(BYTE rawValue, float threshold, float max)
{
    return rawValue > threshold ? rawValue / max : 0.0f;
}

float DirectXInput::CalculateAxisValue(SHORT rawValue, float deadzone, float left, float right)
{
    return abs(rawValue) > deadzone ? (-1.0f + (rawValue - left) * (2.0f / (right - left))) : 0.0f;
}
