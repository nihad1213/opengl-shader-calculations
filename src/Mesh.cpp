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


Mesh makeCube(float size) {
    const float h = size * 0.5f;
    const float faces[6][9] = {
        { 0, 0, 1,   1, 0, 0,   0, 1, 0},
        { 0, 0,-1,  -1, 0, 0,   0, 1, 0},
        { 1, 0, 0,   0, 0,-1,   0, 1, 0},
        {-1, 0, 0,   0, 0, 1,   0, 1, 0},
        { 0, 1, 0,   1, 0, 0,   0, 0,-1},
        { 0,-1, 0,   1, 0, 0,   0, 0, 1},
    };
    const float corners[4][2] = {{-1, -1}, {1, -1}, {1, 1}, {-1, 1}};

    std::vector<Vertex> v;
    std::vector<unsigned> idx;
    for (auto &F : faces) {
        unsigned base = (unsigned)v.size();
        for (auto &c : corners)
            v.push_back({(F[0] + F[3] * c[0] + F[6] * c[1]) * h,
                         (F[1] + F[4] * c[0] + F[7] * c[1]) * h,
                         (F[2] + F[5] * c[0] + F[8] * c[1]) * h,
                         F[0], F[1], F[2]});
        idx.insert(idx.end(), {base, base + 1, base + 2, base, base + 2, base + 3});
    }
    return createMesh(v, idx);
}

Mesh makeCylinder(float r, float height, int seg) {
    std::vector<Vertex> v;
    std::vector<unsigned> idx;
    const float h = height * 0.5f;

    for (int j = 0; j <= seg; ++j) {
        float a = 2 * PI * j / seg, c = std::cos(a), s = std::sin(a);
        v.push_back({r * c, -h, r * s, c, 0, s});
        v.push_back({r * c,  h, r * s, c, 0, s});
    }
    for (int j = 0; j < seg; ++j) {
        unsigned a = j * 2;
        idx.insert(idx.end(), {a, a + 1, a + 2, a + 2, a + 1, a + 3});
    }

    for (int cap = 0; cap < 2; ++cap) {
        float y = cap ? h : -h, ny = cap ? 1.0f : -1.0f;
        unsigned center = (unsigned)v.size();
        v.push_back({0, y, 0, 0, ny, 0});
        for (int j = 0; j <= seg; ++j) {
            float a = 2 * PI * j / seg;
            v.push_back({r * std::cos(a), y, r * std::sin(a), 0, ny, 0});
        }
        for (int j = 0; j < seg; ++j)
            idx.insert(idx.end(), {center, center + 1 + j, center + 2 + j});
    }
    return createMesh(v, idx);
}