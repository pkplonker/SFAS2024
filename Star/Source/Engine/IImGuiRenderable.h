#pragma once
#include "string"
class IImGuiRenderable
{
public:
    virtual ~IImGuiRenderable() = default;
    virtual void Render(std::string window) = 0;
};
