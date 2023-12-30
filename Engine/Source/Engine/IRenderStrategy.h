#pragma once

class IRenderStrategy
{
public:
    virtual ~IRenderStrategy() = default;
    virtual void Update() =0;
};
