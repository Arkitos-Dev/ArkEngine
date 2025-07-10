// Light.hpp
#pragma once
#include "glm/glm.hpp"
#include "../core/Shader.hpp"

class Light : public GameObject {
public:
    enum class Type { Directional, Point, Spot };
    Type type;
    glm::vec3 color = {1, 1, 1};
    glm::vec3 direction = {0, -1, 0};
    float constant = 1.0f, linear = 0.09f, quadratic = 0.032f;
    float cutOff = 0.0f, outerCutOff = 0.0f;

    Light(Type t) : type(t) {}
    virtual ~Light() = default;

    virtual void UploadToShader(Shader* shader, int index = -1) const = 0;
};