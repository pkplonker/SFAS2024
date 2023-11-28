#pragma once
#include <string>
#include <vector>

#include "Vertex.h"

class Mesh
{
public:
    Mesh(std::string path);
    std::vector<Vertex> Vertices = {};
    std::vector<unsigned int> Indices = {};
    std::string GetPath() { return path; }
    protected:
        std::string path;
};
