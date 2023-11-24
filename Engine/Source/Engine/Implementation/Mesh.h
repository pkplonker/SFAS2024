#pragma once
#include <vector>

#include "Vertex.h"

class Mesh
{
public:
    Mesh();
    std::vector<Vertex> Vertices = {};
    std::vector<unsigned int> Indices = {};
};
