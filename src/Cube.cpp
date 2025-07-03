//
// Created by Anton on 03.07.2025.
//

#include "../inlcude/Cube.hpp"

// 8 Ecken, 36 Indices (12 Dreiecke)
static const float cubeVertices[] = {
        // Position           // Normale
        -0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,
        0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,
        0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,
        0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,
};

static const unsigned int cubeIndices[] = {
        // Rückseite
        0, 1, 2, 2, 3, 0,
        // Vorderseite
        4, 5, 6, 6, 7, 4,
        // Links
        0, 4, 7, 7, 3, 0,
        // Rechts
        1, 5, 6, 6, 2, 1,
        // Unten
        0, 1, 5, 5, 4, 0,
        // Oben
        3, 2, 6, 6, 7, 3
};

Cube::Cube()
        : Mesh(cubeVertices, sizeof(cubeVertices), cubeIndices, sizeof(cubeIndices)) {}
