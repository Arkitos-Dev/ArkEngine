//
// Created by Anton on 09.07.2025.
//

#ifndef INC_3DRENDERER_MESHNEW_HPP
#define INC_3DRENDERER_MESHNEW_HPP

#include "glm/glm.hpp"
#include <string>
#include <vector>
#include "../core/Shader.hpp"

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords; // <- vec2!
};

struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};

class MeshNew {
public:
    std::vector<Vertex>       vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture>      textures;
    std::vector<Texture> textures_loaded;
    unsigned int VAO;

    MeshNew(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
    void DrawInstanced(Shader& shader);
    void SetModelMatrices(const std::vector<glm::mat4> &matrices);
private:
    unsigned int VBO, EBO;
    unsigned int instanceVBO = 0;
    size_t instanceCount = 0;
    void SetupMesh();
};

#endif //INC_3DRENDERER_MESHNEW_HPP
