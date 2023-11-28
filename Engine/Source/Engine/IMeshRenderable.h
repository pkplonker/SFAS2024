#pragma once
#include "IRenderable.h"

class IMeshRenderable : public IRenderable
{
public:
    IMeshRenderable(const std::string& path);
    virtual std::string GetPath() { return path; }

protected:
    std::string path;
};
