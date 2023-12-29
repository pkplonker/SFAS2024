#pragma once
#include "IRenderable.h"

class IMeshRenderable : public IRenderable
{
public:
    IMeshRenderable(const std::string& path);
    virtual std::string GetPath() { return path; }
    unsigned int GetVertsCount() override =0;
    unsigned int GetTriangleCount() override  =0;

protected:
    std::string path;
};
