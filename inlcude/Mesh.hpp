//
// Created by Anton on 03.07.2025.
//

#ifndef INC_3DRENDERER_MESH_HPP
#define INC_3DRENDERER_MESH_HPP
#include <cstddef>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Mesh {
public:
    Mesh(const float* vertices, size_t vertSize, const unsigned int* indices, size_t idxSize, const char* texturePath1 = nullptr, const char* texturePath2 = nullptr);
    ~Mesh();
    void setPosition(const glm::vec3& pos) { position = pos; updateModelMatrix(); }
    glm::mat4 getModelMatrix() const { return model; }
    void bind() const;
    void unbind() const;
    void draw() const;
private:
    glm::mat4 model = glm::mat4(1.0f);
    glm::vec3 position = glm::vec3(0.0f);
    void updateModelMatrix() { model = glm::translate(glm::mat4(1.0f), position); }
    unsigned int VAO, VBO, EBO, texture1, texture2;
    size_t indexCount;
};

#endif //INC_3DRENDERER_MESH_HPP
