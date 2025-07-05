//
// Created by Anton on 03.07.2025.
//

#ifndef INC_3DRENDERER_MESH_HPP
#define INC_3DRENDERER_MESH_HPP
#include <cstddef>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"
#include <vector>
#include <string>

class Mesh {
public:
    Mesh(const float* vertices, size_t vertSize, const unsigned int* indices, size_t idxSize, const char* texturePath1 = nullptr, const char* texturePath2 = nullptr);
    ~Mesh();

    void SetPosition(const glm::vec3& pos) { position = pos;
        UpdateModelMatrix(); }
    void SetRotation(float angleDeg, const glm::vec3& axis) { rotation = glm::angleAxis(glm::radians(angleDeg), glm::normalize(axis));
        UpdateModelMatrix(); }
    void SetScale(const glm::vec3& s) { scale = s;
        UpdateModelMatrix(); }

    void SetModelMatrices(const std::vector<glm::mat4>& matrices);
    size_t GetInstanceCount() const { return instanceCount; }

    void SetPrototype(Mesh* proto) { prototype = proto; }
    Mesh* GetPrototype() const { return prototype ? prototype : const_cast<Mesh*>(this); }

    void SetTexture(const char* texturePath);
    unsigned int GetTextureID() const { return texture1; }

    glm::mat4 GetModelMatrix() const { return model; }
    void Bind() const;
    void Unbind() const;
    void Draw() const;
    void DrawInstanced() const;

    std::string name;
    Mesh(const std::string& name = "Mesh") : name(name) {}
private:
    glm::mat4 model = glm::mat4(1.0f);
    glm::vec3 position = glm::vec3(0.0f);
    glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    glm::vec3 scale = glm::vec3(1.0f);

    unsigned int VAO, VBO, EBO, texture1, texture2, instanceVBO = 0;
    size_t indexCount, instanceCount = 0;

    Mesh* prototype = nullptr;

    void SetupBuffers(const float* vertices, size_t vertSize, const unsigned int* indices, size_t idxSize);
    void LoadTextures(const char* texturePath1, const char* texturePath2);

    void UpdateModelMatrix() {
        model = glm::translate(glm::mat4(1.0f), position)
                * glm::mat4_cast(rotation)
                * glm::scale(glm::mat4(1.0f), scale);
    }
};

#endif //INC_3DRENDERER_MESH_HPP
