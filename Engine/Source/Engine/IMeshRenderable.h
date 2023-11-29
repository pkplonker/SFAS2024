#pragma once
#include "IRenderable.h"

class IMeshRenderable : public IRenderable
{
public:
    IMeshRenderable(const std::string& path);
    virtual std::string GetPath() { return path; }
    unsigned int GetVerts() override =0;
    unsigned int GetTriangles() override  =0;

protected:
    std::string path;
};
