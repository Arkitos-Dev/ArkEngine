//
// Created by Anton on 03.07.2025.
//

#include "../inlcude/Cube.hpp"

float cubeVertices[] = {
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
        -0.5f,  0.5f, -0.5f,  0, 1, 0,  0.0f, 0.0f,
};
unsigned int cubeIndices[] = {
        0, 1, 2, 0, 2, 3,    // Rückseite
        4, 5, 6, 4, 6, 7,    // Vorderseite
        8, 9,10, 8,10,11,    // Links
        12,13,14,12,14,15,   // Rechts
        16,17,18,16,18,19,   // Unten
        20,21,22,20,22,23    // Oben
};


Cube::Cube()
        : Mesh(cubeVertices, sizeof(cubeVertices), cubeIndices, sizeof(cubeIndices), "resources/images/container.jpg", "resources/images/awesomeface.png") {}
