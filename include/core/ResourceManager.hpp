//
// Created by Anton on 05.07.2025.
//
#pragma once
#include <string>
#include <map>
#include "Shader.hpp"

class ResourceManager {
public:
    static unsigned int GetTexture(const std::string& path);
    static void Clear();

    static Shader* GetShader(const std::string& vertexPath, const std::string& fragmentPath);
    static void ClearShaders();

private:
    static std::map<std::string, unsigned int> textures;
    static std::map<std::string, Shader*> shaders;
};