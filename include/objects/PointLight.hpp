//
// Created by Anton on 07.07.2025.
//
#pragma once
#include "Light.hpp"
#include <string>

class PointLight : public Light {
public:
    PointLight() : Light(Type::Point) {}

    void UploadToShader(Shader* shader, int index) const override {
        std::string idx = "pointLights[" + std::to_string(index) + "]";
        shader->SetVec3(idx + ".position", position);
        shader->SetVec3(idx + ".ambient",  color * 0.1f);
        shader->SetVec3(idx + ".diffuse",  color * 0.8f);
        shader->SetVec3(idx + ".specular", color * 1.0f);
        shader->SetFloat(idx + ".constant",  constant);
        shader->SetFloat(idx + ".linear",    linear);
        shader->SetFloat(idx + ".quadratic", quadratic);
    }
};