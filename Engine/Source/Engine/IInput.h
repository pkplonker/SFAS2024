#pragma once
#include "Audio.h"

struct Vec2;

enum InputAction
{
    ButtonLeft,
    ButtonRight,
    ButtonTop,
    ButtonBottom,

    DirectionPadLeft,
    DirectionPadRight,
    DirectionPadTop,
    DirectionPadBottom,

    LeftStickXAxis,
    LeftStickYAxis,
    LeftStickPress,

    RightStickXAxis,
    RightStickYAxis,
    RightStickPress,

    TriggerLeft,
    TriggerRight,
    ShoulderButtonLeft,
    ShoulderButtonRight,

    SpecialLeft,
    SpecialRight,

    NumActions,
};

class IInput
{
public:
    IInput();

    virtual ~IInput() = default;

    virtual void Update();

    virtual float GetValue(InputAction action) const;
    virtual bool IsPressed(InputAction action, float threshold = 0.5f) const;
    virtual bool IsReleased(InputAction action, float threshold = 0.5f) const;
    virtual bool IsHeld(InputAction action, float threshold = 0.5f) const;
    virtual void ProcessMouse(UINT msg, WPARAM uint, LPARAM lparam) const =0;
    virtual void ProcessKeyboard(UINT msg, WPARAM uint, LPARAM lparam) const =0;
    virtual bool IsLeftPressed() const =0;
    virtual bool IsLeftHeld() const =0;
    virtual bool IsLeftReleased() const =0;

    virtual bool IsRightPressed() const =0;
    virtual bool IsRightHeld() const =0;
    virtual bool IsRightReleased() const =0;

    virtual bool IsMiddlePressed() const =0;
    virtual bool IsMiddleHeld() const =0;
    virtual bool IsMiddleReleased() const =0;

    virtual int GetMouseX() const =0;
    virtual int GetMouseY() const =0;
    virtual int GetDeltaX() const =0;
    virtual int GetDeltaY() const =0;
    virtual Vec2 GetMousePosition() =0;
    virtual Vec2 GetMouseDelta() =0;
    static const unsigned int NumInputActions = static_cast<unsigned int>(NumActions);

protected:
    float CurrentState[NumInputActions];
    float PreviousState[NumInputActions];
};
