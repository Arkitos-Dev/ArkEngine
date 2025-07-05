//
// Created by Anton on 03.07.2025.
//
#ifndef INC_3DRENDERER_SHADER_HPP
#define INC_3DRENDERER_SHADER_HPP

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
    unsigned int ID;
    Shader(const char* vertexPath, const char* fragmentPath);
    void Use();
    void SetBool(const std::string &name, bool value) const;
    void SetInt(const std::string &name, int value) const;
    void SetFloat(const std::string &name, float value) const;
    void SetMat4(const std::string& name, const glm::mat4& mat) const;
    void SetVec3(const std::string& name, const glm::vec3& vec) const;
private:
    std::string LoadShaderCode(const char* path);
    unsigned int CompileShader(const char* code, int type);
    void LinkProgram(unsigned int vertex, unsigned int fragment);
};

#endif //INC_3DRENDERER_SHADER_HPP