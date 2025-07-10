//
// Created by Anton on 04.07.2025.
//
#pragma once
#include "Mesh.hpp"

class Plane : public Mesh {
public:
    Plane(const std::vector<Texture>& textures = {})
            : Mesh(GetVertices(), GetIndices(), textures) {}

private:
    static std::vector<Vertex> GetVertices() {
        static const float rawVertices[] = {
                // Position         // Normal      // TexCoords
                -0.5f, 0.0f, -0.5f,  0,1,0,   0.0f, 0.0f,
                0.5f, 0.0f, -0.5f,  0,1,0,   1.0f, 0.0f,
                0.5f, 0.0f,  0.5f,  0,1,0,   1.0f, 1.0f,
                -0.5f, 0.0f,  0.5f,  0,1,0,   0.0f, 1.0f
        };
        std::vector<Vertex> vertices;
        for (size_t i = 0; i < 4; ++i) {
            Vertex v;
            v.position = glm::vec3(rawVertices[i * 8 + 0], rawVertices[i * 8 + 1], rawVertices[i * 8 + 2]);
            v.normal   = glm::vec3(rawVertices[i * 8 + 3], rawVertices[i * 8 + 4], rawVertices[i * 8 + 5]);
            v.texCoords= glm::vec2(rawVertices[i * 8 + 6], rawVertices[i * 8 + 7]);
            vertices.push_back(v);
        }
        return vertices;
    }

    static std::vector<unsigned int> GetIndices() {
        static const unsigned int idx[] = {
                0, 1, 2,
                2, 3, 0
        };
        return std::vector<unsigned int>(idx, idx + 6);
    }
};
