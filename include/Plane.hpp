//
// Created by Anton on 04.07.2025.
//

#ifndef INC_3DRENDERER_PLANE_HPP
#define INC_3DRENDERER_PLANE_HPP

#include "Mesh.hpp"

class Plane : public Mesh {
public:
    Plane(const char* texturePath = "resources/images/container.jpg")
            : Mesh(vertices, sizeof(vertices), indices, sizeof(indices), texturePath) {}

private:
    // statische Vertex- und Indexdaten für ein Quadrat im XZ-Plane
    static constexpr float vertices[] = {
            // Position         // Normal         // TexCoords
            -0.5f, 0.0f, -0.5f,  0,1,0,   0.0f, 0.0f,
            0.5f, 0.0f, -0.5f,  0,1,0,   1.0f, 0.0f,
            0.5f, 0.0f,  0.5f,  0,1,0,   1.0f, 1.0f,
            -0.5f, 0.0f,  0.5f,  0,1,0,   0.0f, 1.0f
    };
    static constexpr unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
    };
};

#endif //INC_3DRENDERER_PLANE_HPP
