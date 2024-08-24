#pragma once

class IResizeHandler {
public:
    virtual ~IResizeHandler() = default;
    virtual void WindowResize(int width, int height) = 0;
};