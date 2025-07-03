//
// Created by Anton on 03.07.2025.
//

#ifndef INC_3DRENDERER_MESH_HPP
#define INC_3DRENDERER_MESH_HPP
#include <cstddef>

class Mesh {
public:
    Mesh(const float* vertices, size_t vertSize, const unsigned int* indices, size_t idxSize);
    ~Mesh();
    void bind() const;
    void unbind() const;
    void draw() const;
private:
    unsigned int VAO, VBO, EBO;
    size_t indexCount;
};

#endif //INC_3DRENDERER_MESH_HPP
