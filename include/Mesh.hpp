//
// Created by Anton on 03.07.2025.
//

#ifndef INC_3DRENDERER_MESH_HPP
#define INC_3DRENDERER_MESH_HPP
#include <cstddef>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <vector>
#include <string>

class Mesh {
public:
    Mesh(const float* vertices, size_t vertSize, const unsigned int* indices, size_t idxSize, const char* texturePath1 = nullptr, const char* texturePath2 = nullptr);
    ~Mesh();

    void setPosition(const glm::vec3& pos) { position = pos; updateModelMatrix(); }
    void setRotation(float angleDeg, const glm::vec3& axis) { rotation = glm::angleAxis(glm::radians(angleDeg), glm::normalize(axis)); updateModelMatrix(); }
    void setScale(const glm::vec3& s) { scale = s; updateModelMatrix(); }

    void setInstanceModelMatrices(const std::vector<glm::mat4>& matrices);
    size_t getInstanceCount() const { return instanceCount; }

    void setPrototype(Mesh* proto) { prototype = proto; }
    Mesh* getPrototype() const { return prototype ? prototype : const_cast<Mesh*>(this); }

    glm::mat4 getModelMatrix() const { return model; }
    void bind() const;
    void unbind() const;
    void draw() const;
    void drawInstanced() const;

    std::string name;
    Mesh(const std::string& name = "Mesh") : name(name) {}
private:
    glm::mat4 model = glm::mat4(1.0f);
    glm::vec3 position = glm::vec3(0.0f);
    glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    glm::vec3 scale = glm::vec3(1.0f);

    Mesh* prototype = nullptr;

    void updateModelMatrix() {
        model = glm::translate(glm::mat4(1.0f), position)
                * glm::mat4_cast(rotation)
                * glm::scale(glm::mat4(1.0f), scale);
    }
    unsigned int VAO, VBO, EBO, texture1, texture2, instanceVBO = 0;
    size_t indexCount, instanceCount = 0;
};

#endif //INC_3DRENDERER_MESH_HPP
