//
// Created by Anton on 06.07.2025.
//
#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "glm/glm.hpp"
#include "../core/Shader.hpp"

enum class LightType {
    DIRECTIONAL,
    POINT,
    SPOT
};

class Light {
public:
    Light(LightType type);
    ~Light() = default;

    // Setters
    void SetPosition(const glm::vec3& pos) { position = pos; }
    void SetDirection(const glm::vec3& dir) { direction = dir; }
    void SetAmbient(const glm::vec3& amb) { ambient = amb; }
    void SetDiffuse(const glm::vec3& diff) { diffuse = diff; }
    void SetSpecular(const glm::vec3& spec) { specular = spec; }
    void SetAttenuation(float constant, float linear, float quadratic);
    void SetSpotlight(float cutOff, float outerCutOff);

    // Getters
    LightType GetType() const { return type; }
    const glm::vec3& GetPosition() const { return position; }
    const glm::vec3& GetDirection() const { return direction; }
    const glm::vec3& GetAmbient() const { return ambient; }
    const glm::vec3& GetDiffuse() const { return diffuse; }
    const glm::vec3& GetSpecular() const { return specular; }

    // Shader Upload
    void UploadToShader(Shader* shader, const std::string& uniformName) const;

private:
    LightType type;
    glm::vec3 position{0.0f};
    glm::vec3 direction{0.0f, -1.0f, 0.0f};
    glm::vec3 ambient{0.1f};
    glm::vec3 diffuse{1.0f};
    glm::vec3 specular{1.0f};


    float constant = 1.0f;
    float linear = 0.007f;      // Statt 0.045f
    float quadratic = 0.0002f;  // Statt 0.0075f

    // Spotlight
    float cutOff = glm::cos(glm::radians(12.5f));
    float outerCutOff = glm::cos(glm::radians(15.0f));
};

#endif
