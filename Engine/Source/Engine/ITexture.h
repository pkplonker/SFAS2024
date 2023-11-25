#pragma once
#include <string>

class ITexture
{
public:
    virtual ~ITexture() = default;
    ITexture(std::wstring path) : path(std::move(path)){}

    virtual void Update() = 0;
    virtual float GetWidth() const = 0;
    virtual float GetHeight() const = 0;
    virtual std::wstring GetPath(){return path;}
protected:
    std::wstring path;
};
