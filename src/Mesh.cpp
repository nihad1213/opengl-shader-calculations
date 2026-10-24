#include "Mesh.hpp"
#include <cmath>

static const float PI = 3.14159265f;

Mesh createMesh(const std::vector<Vertex> &v, const std::vector<unsigned> &idx) {
    Mesh m;
    m.count = (GLsizei)idx.size();
    glGenVertexArrays(1, &m.vao);
    glGenBuffers(1, &m.vbo);
    glGenBuffers(1, &m.ebo);

    glBindVertexArray(m.vao);
    glBindBuffer(GL_ARRAY_BUFFER, m.vbo);
    glBufferData(GL_ARRAY_BUFFER, v.size() * sizeof(Vertex), v.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, idx.size() * sizeof(unsigned), idx.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);                  // position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(3 * sizeof(float))); // normal
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
    return m;
}

void Mesh::draw() const {
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

Mesh makeSphere(float r, int stacks, int slices) {
    std::vector<Vertex> v;
    std::vector<unsigned> idx;

    for (int i = 0; i <= stacks; ++i) {
        float theta = PI * i / stacks;
        for (int j = 0; j <= slices; ++j) {
            float phi = 2 * PI * j / slices;
            float nx = std::sin(theta) * std::cos(phi);
            float ny = std::cos(theta);
            float nz = std::sin(theta) * std::sin(phi);
            v.push_back({nx * r, ny * r, nz * r, nx, ny, nz});
        }
    }
    for (int i = 0; i < stacks; ++i)
        for (int j = 0; j < slices; ++j) {
            unsigned a = i * (slices + 1) + j, b = a + slices + 1;
            idx.insert(idx.end(), {a, b, a + 1, a + 1, b, b + 1});
        }
    return createMesh(v, idx);
}