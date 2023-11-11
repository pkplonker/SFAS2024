#pragma once

class GameObject;

class IUpdateable
{
public:
    virtual ~IUpdateable() = default;
    virtual void Update() = 0;
};
