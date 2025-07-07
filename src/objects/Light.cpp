//
// Created by Anton on 06.07.2025.
//
#include "../../include/objects/Light.hpp"

Light::Light(LightType type) : type(type) {
    // Standard-Werte je nach Typ
    switch(type) {
        case LightType::DIRECTIONAL:
            direction = glm::vec3(-0.2f, -1.0f, -0.3f);
            ambient = glm::vec3(0.3f);
            diffuse = glm::vec3(0.8f);
            specular = glm::vec3(1.0f);
            break;
        case LightType::POINT:
            ambient = glm::vec3(0.1f);
            diffuse = glm::vec3(1.0f);
            specular = glm::vec3(1.0f);
            break;
        case LightType::SPOT:
            ambient = glm::vec3(0.0f);
            diffuse = glm::vec3(1.0f);
            specular = glm::vec3(1.0f);
            break;
    }
}

void Light::SetAttenuation(float constant, float linear, float quadratic) {
    this->constant = constant;
    this->linear = linear;
    this->quadratic = quadratic;
}

void Light::SetSpotlight(float cutOff, float outerCutOff) {
    this->cutOff = glm::cos(glm::radians(cutOff));
    this->outerCutOff = glm::cos(glm::radians(outerCutOff));
}

void Light::UploadToShader(Shader* shader, const std::string& uniformName) const {
    switch (type) {
        case LightType::DIRECTIONAL:
            shader->SetVec3((uniformName + ".direction").c_str(), direction);
            shader->SetVec3((uniformName + ".ambient").c_str(), ambient);
            shader->SetVec3((uniformName + ".diffuse").c_str(), diffuse);
            shader->SetVec3((uniformName + ".specular").c_str(), specular);
            break;

        case LightType::POINT:
            shader->SetVec3((uniformName + ".position").c_str(), position);
            shader->SetVec3((uniformName + ".ambient").c_str(), ambient);
            shader->SetVec3((uniformName + ".diffuse").c_str(), diffuse);
            shader->SetVec3((uniformName + ".specular").c_str(), specular);
            shader->SetFloat((uniformName + ".constant").c_str(), constant);
            shader->SetFloat((uniformName + ".linear").c_str(), linear);
            shader->SetFloat((uniformName + ".quadratic").c_str(), quadratic);
            break;

        case LightType::SPOT:
            shader->SetVec3((uniformName + ".position").c_str(), position);
            shader->SetVec3((uniformName + ".direction").c_str(), direction);
            shader->SetVec3((uniformName + ".ambient").c_str(), ambient);
            shader->SetVec3((uniformName + ".diffuse").c_str(), diffuse);
            shader->SetVec3((uniformName + ".specular").c_str(), specular);
            shader->SetFloat((uniformName + ".constant").c_str(), constant);
            shader->SetFloat((uniformName + ".linear").c_str(), linear);
            shader->SetFloat((uniformName + ".quadratic").c_str(), quadratic);
            shader->SetFloat((uniformName + ".cutOff").c_str(), cutOff);        // Bereits konvertiert
            shader->SetFloat((uniformName + ".outerCutOff").c_str(), outerCutOff);
            break;
    }
}