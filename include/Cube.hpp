//
// Created by Anton on 03.07.2025.
//

#ifndef INC_3DRENDERER_CUBE_HPP
#define INC_3DRENDERER_CUBE_HPP

#include "Mesh.hpp"

class Cube : public Mesh {
public:
    Cube(const char* texturePath = "resources/images/container.jpg")
            : Mesh( vertices, sizeof(vertices), indices, sizeof(indices), texturePath) {}
private:
    static constexpr float vertices[] = {
            // Rückseite
            0.5f, -0.5f, -0.5f,  0, 0,-1,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0, 0,-1,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0, 0,-1,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  0, 0,-1,  0.0f, 0.0f,

            // Vorderseite
            -0.5f, -0.5f,  0.5f,  0, 0, 1,  0.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  0, 0, 1,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  0, 0, 1,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0, 0, 1,  0.0f, 0.0f,

            // Links
            -0.5f, -0.5f, -0.5f, -1, 0, 0,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f, -1, 0, 0,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f, -1, 0, 0,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f, -1, 0, 0,  0.0f, 0.0f,

            // Rechts
            0.5f, -0.5f,  0.5f,  1, 0, 0,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  1, 0, 0,  1.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  1, 0, 0,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1, 0, 0,  0.0f, 0.0f,

            // Unten
            -0.5f, -0.5f, -0.5f,  0,-1, 0,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0,-1, 0,  1.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  0,-1, 0,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0,-1, 0,  0.0f, 0.0f,

            // Oben
            -0.5f,  0.5f,  0.5f,  0, 1, 0,  0.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  0, 1, 0,  1.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  0, 1, 0,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0, 1, 0,  0.0f, 0.0f
    };

    static constexpr unsigned int indices[] = {
            0, 1, 2, 0, 2, 3,    // Rückseite
            4, 5, 6, 4, 6, 7,    // Vorderseite
            8, 9,10, 8,10,11,    // Links
            12,13,14,12,14,15,   // Rechts
            16,17,18,16,18,19,   // Unten
            20,21,22,20,22,23    // Oben
    };
};

#endif //INC_3DRENDERER_CUBE_HPP
