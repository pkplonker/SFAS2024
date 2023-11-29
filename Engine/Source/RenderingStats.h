#pragma once

struct RenderingStats
{
    unsigned int drawCalls = 0;
    unsigned int materials = 0;
    unsigned int shaders = 0;
    unsigned int tris = 0;
    unsigned int verts = 0;
    unsigned int batches = 0;
    unsigned int width = 0;
    unsigned int height = 0;
    unsigned int viewportWidth = 0;
    unsigned int viewportHeight = 0;
};
