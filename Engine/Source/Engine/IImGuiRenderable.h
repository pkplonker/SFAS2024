#pragma once
#include "string"
class IImGuiRenderable
{
public:
    virtual ~IImGuiRenderable() = default;
    virtual void ImGuiRender(std::string window) = 0;
};
