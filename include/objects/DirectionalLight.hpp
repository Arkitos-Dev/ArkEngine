//
// Created by Anton on 07.07.2025.
//
#pragma once
#include "Light.hpp"

class DirectionalLight : public Light {
public:
    DirectionalLight() : Light(Type::Directional) {}

    void UploadToShader(Shader* shader, int index = -1) const override {
        shader->SetVec3("dirLight.direction", direction);
        shader->SetVec3("dirLight.ambient",  color * 0.1f);
        shader->SetVec3("dirLight.diffuse",  color * 0.8f);
        shader->SetVec3("dirLight.specular", color * 1.0f);
    }
};
