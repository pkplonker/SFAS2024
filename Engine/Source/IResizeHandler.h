#pragma once

class IResizeHandler {
public:
    virtual ~IResizeHandler() = default;
    virtual void Resize(int width, int height) = 0;
};