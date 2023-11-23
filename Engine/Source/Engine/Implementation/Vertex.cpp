#include "Vertex.h"

bool Vertex::operator == (const Vertex& vert) {
    return Compare(vert);
}
bool Vertex::Compare(const Vertex& vertex)
{
    if (color != vertex.color)return false;
    if (position != vertex.position)return false;
    if (uv != vertex.uv)return false;
    if (normal != vertex.normal)return false;
    return true;
}
