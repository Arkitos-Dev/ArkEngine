#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <ostream>
#include <istream>

class GameObject {
public:
    virtual ~GameObject() = default;

    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 scale = glm::vec3(1.0f);
    glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);

    // Position
    void SetPosition(const glm::vec3& pos) { position = pos; }
    void Move(const glm::vec3& offset) { position += offset; }
    glm::vec3 GetPosition() const { return position; }

    // Skalierung
    void SetScale(const glm::vec3& s) { scale = s; }
    void Scale(const glm::vec3& factor) { scale *= factor; }
    glm::vec3 GetScale() const { return scale; }

    // Rotation
    void SetRotation(const glm::quat& rot) { rotation = rot; }
    void SetRotation(float angleDeg, const glm::vec3& axis) {
        rotation = glm::angleAxis(glm::radians(angleDeg), glm::normalize(axis));
    }
    void Rotate(float angleDeg, const glm::vec3& axis) {
        rotation = glm::normalize(glm::angleAxis(glm::radians(angleDeg), glm::normalize(axis)) * rotation);
    }
    glm::quat GetRotation() const { return rotation; }
};