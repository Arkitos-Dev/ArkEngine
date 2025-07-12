//
// Created by Anton on 05.07.2025.
//
#pragma once
#include <string>
#include <map>
#include <memory>
#include "Shader.hpp"
#include "../objects/Model.hpp"

class Model;

class ResourceManager {
public:
    static unsigned int GetTexture(const std::string& path);
    static void ClearTextures();

    static std::shared_ptr<Shader> GetShader(const std::string& vertexPath, const std::string& fragmentPath);
    static void ClearShaders();

    static std::shared_ptr<Model> GetModel(const std::string& path);
    static void ClearModels();


private:
    static std::map<std::string, unsigned int> textures;
    static std::map<std::string, std::shared_ptr<Shader>> shaders;
    static std::map<std::string, std::shared_ptr<Model>> models;

};