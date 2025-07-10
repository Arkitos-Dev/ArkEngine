//
// Created by Anton on 05.07.2025.
//
#include "../../include/core/ResourceManager.hpp"
#include "stb_image.h"
#include "glad/glad.h"

std::map<std::string, unsigned int> ResourceManager::textures;
std::map<std::string, std::shared_ptr<Shader>> ResourceManager::shaders;
std::map<std::string, std::shared_ptr<Model>> ResourceManager::models;

unsigned int ResourceManager::GetTexture(const std::string& path) {
    if (textures.count(path)) return textures[path];

    int w, h, c;
    unsigned char* data = stbi_load(path.c_str(), &w, &h, &c, 0);
    if (!data) return 0;

    unsigned int tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    GLenum format = (c == 4) ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
    textures[path] = tex;
    return tex;
}

void ResourceManager::ClearTextures() {
    for (auto& [_, tex] : textures) glDeleteTextures(1, &tex);
    textures.clear();
}

std::shared_ptr<Shader> ResourceManager::GetShader(const std::string& vertexPath, const std::string& fragmentPath) {
    std::string key = vertexPath + "|" + fragmentPath;
    auto it = shaders.find(key);
    if (it != shaders.end()) return it->second;
    auto shader = std::make_shared<Shader>(vertexPath.c_str(), fragmentPath.c_str());
    shaders[key] = shader;
    return shader;
}

void ResourceManager::ClearShaders() {
    shaders.clear();
}

std::shared_ptr<Model> ResourceManager::GetModel(const std::string& path) {
    auto it = models.find(path);
    if (it != models.end()) return it->second;
    auto model = std::make_shared<Model>(path);
    models[path] = model;
    return model;
}

void ResourceManager::ClearModels() {
    models.clear();
}