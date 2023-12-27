#include "DirectXInput.h"

#include <iostream>
#include <math.h>

#include "EngineTime.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Math/Vector2.h"

DirectXInput::DirectXInput(HWND window) : IInput(), State()
{
    keyboard = std::make_unique<DirectX::Keyboard>();
    mouse = std::make_unique<DirectX::Mouse>();
    mouse->SetWindow(window);
    auto state = mouse->GetState();
}

DirectXInput::~DirectXInput()
{
}

void DirectXInput::ProcessMouse(UINT msg, WPARAM wParam, LPARAM lParam) const
{
    DirectX::Mouse::ProcessMessage(msg, wParam, lParam);
    mouseDataUpdated = true;
}

void DirectXInput::ProcessKeyboard(UINT msg, WPARAM wParam, LPARAM lParam) const
{
    DirectX::Keyboard::ProcessMessage(msg, wParam, lParam);
    keyboardDataUpdated = true;
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
    // CurrentState[static_cast<unsigned int>(InputAction::SpecialLeft)] = State.Gamepad.wButtons & XINPUT_GAMEPAD_BACK
    //                                                                         ? 1.0f
    //                                                                         : 0.0f;
    // CurrentState[static_cast<unsigned int>(InputAction::SpecialRight)] = State.Gamepad.wButtons & XINPUT_GAMEPAD_START
    //                                                                          ? 1.0f
    //                                                                          : 0.0f;

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
    if (mouse == nullptr)return;
    auto state = mouse->GetState();
    tracker.Update(state);

    // Update button states
    leftPressed = tracker.leftButton == DirectX::Mouse::ButtonStateTracker::PRESSED;
    leftHeld = tracker.leftButton == DirectX::Mouse::ButtonStateTracker::HELD;
    leftReleased = tracker.leftButton == DirectX::Mouse::ButtonStateTracker::RELEASED;

    rightPressed = tracker.rightButton == DirectX::Mouse::ButtonStateTracker::PRESSED;
    rightHeld = tracker.rightButton == DirectX::Mouse::ButtonStateTracker::HELD;
    rightReleased = tracker.rightButton == DirectX::Mouse::ButtonStateTracker::RELEASED;

    middlePressed = tracker.middleButton == DirectX::Mouse::ButtonStateTracker::PRESSED;
    middleHeld = tracker.middleButton == DirectX::Mouse::ButtonStateTracker::HELD;
    middleReleased = tracker.middleButton == DirectX::Mouse::ButtonStateTracker::RELEASED;

    mouseX = state.x;
    mouseY = state.y;
    deltaX = state.x - lastX;
    deltaY = state.y - lastY;

    lastX = state.x;
    lastY = state.y;

    scrollDelta = state.scrollWheelValue - lastScrollValue;
    lastScrollValue = state.scrollWheelValue;
}

void DirectXInput::HandleKeyboard()
{
    keyboardState = keyboard->GetState();
}

bool DirectXInput::IsKeyDown(Keys key) const
{
    return keyboardState.IsKeyDown(static_cast<DirectX::Keyboard::Keys>(key));
}

void DirectXInput::Update()
{
    IInput::Update();
    HandleController();
    deltaX = 0;
    deltaY = 0;

    HandleMouse();
    HandleKeyboard();
}

bool DirectXInput::IsLeftPressed() const
{
    return leftPressed;
}

bool DirectXInput::IsLeftHeld() const
{
    return leftHeld;
}

bool DirectXInput::IsLeftReleased() const
{
    return leftReleased;
}

bool DirectXInput::IsRightPressed() const
{
    return rightPressed;
}

bool DirectXInput::IsRightHeld() const
{
    return rightHeld;
}

bool DirectXInput::IsRightReleased() const
{
    return rightReleased;
}

bool DirectXInput::IsMiddlePressed() const
{
    return middlePressed;
}

bool DirectXInput::IsMiddleHeld() const
{
    return middleHeld;
}

bool DirectXInput::IsMiddleReleased() const
{
    return middleReleased;
}

int DirectXInput::GetMouseX() const
{
    return mouseX;
}

int DirectXInput::GetMouseY() const
{
    return mouseY;
}

int DirectXInput::GetDeltaX() const
{
    return deltaX;
}

int DirectXInput::GetDeltaY() const
{
    return deltaY;
}


Vec2 DirectXInput::GetMousePosition()
{
    return Vec2(static_cast<float>(mouseX), static_cast<float>(mouseY));
}

Vec2 DirectXInput::GetMouseDelta()
{
    return Vec2(static_cast<float>(lastX) - mouseX, static_cast<float>(lastY) - mouseY);
}

int DirectXInput::GetMouseScrollDelta()
{
    return scrollDelta;
}

float DirectXInput::CalculateTriggerValue(BYTE rawValue, float threshold, float max)
{
    return rawValue > threshold ? rawValue / max : 0.0f;
}

float DirectXInput::CalculateAxisValue(SHORT rawValue, float deadzone, float left, float right)
{
    return abs(rawValue) > deadzone ? (-1.0f + (rawValue - left) * (2.0f / (right - left))) : 0.0f;
}
