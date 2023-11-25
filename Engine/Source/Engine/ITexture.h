#pragma once

class ITexture
{
public:
    virtual ~ITexture() = default;
    virtual void Update() = 0;
    virtual float GetWidth() const = 0;
    virtual float GetHeight() const = 0;
};
