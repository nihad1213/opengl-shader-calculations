#pragma once
#include "glad/glad.h"
#include <vector>

struct Vertex { float px, py, pz, nx, ny, nz; };

struct Mesh {
    GLuint vao = 0, vbo = 0, ebo = 0;
    GLsizei count = 0;
    void draw() const;
};

Mesh createMesh(const std::vector<Vertex> &v, const std::vector<unsigned> &idx);
Mesh makeSphere(float r = 1.0f, int stacks = 48, int slices = 64);
Mesh makeCube(float size = 1.6f);
Mesh makeCylinder(float r = 0.8f, float h = 1.8f, int seg = 64);