//
// Created by Anton on 09.07.2025.
//

#ifndef INC_3DRENDERER_MESH_HPP
#define INC_3DRENDERER_MESH_HPP

#include "glm/glm.hpp"
#include <string>
#include <vector>
#include "../core/Shader.hpp"
#include "../objects/GameObject.hpp"

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

class Mesh : public GameObject {
public:
    std::vector<Vertex>       vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture>      textures;
    std::vector<Texture> textures_loaded;
    unsigned int VAO;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
    void DrawInstanced(Shader& shader);
    void SetModelMatrices(const std::vector<glm::mat4> &matrices);
    Mesh* GetMesh() { return this; }
    const Mesh* GetMesh() const { return this; }
private:
    unsigned int VBO, EBO;
    unsigned int instanceVBO = 0;
    size_t instanceCount = 0;
    void SetupMesh();
};

#endif //INC_3DRENDERER_MESH_HPP
