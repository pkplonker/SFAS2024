#pragma once

class IImGuiRenderable
{
public:
    virtual ~IImGuiRenderable() = default;
    virtual void Render() = 0;
};
