//
// Created by Anton on 03.07.2025.
//
#include "../../include/objects/Mesh.hpp"
#include "../../include/core/ResourceManager.hpp"
#include "glad/glad.h"

Mesh::Mesh(const float* vertices, size_t vertSize, const unsigned int* indices, size_t idxSize, const char* texturePath1, const char* texturePath2)
        : indexCount(idxSize / sizeof(unsigned int)), texture1(0), texture2(0)
{
    SetupBuffers(vertices, vertSize, indices, idxSize);
    LoadTextures(texturePath1, texturePath2);

    // Instancing-Buffer anlegen (leer, wird später befüllt)
    glGenBuffers(1, &instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);

    // Model-Matrix als 4 Attribute (3-6) im VAO registrieren
    glBindVertexArray(VAO);
    for (int i = 0; i < 4; ++i) {
        glVertexAttribPointer(3 + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(float) * i * 4));
        glEnableVertexAttribArray(3 + i);
        glVertexAttribDivisor(3 + i, 1);
    }
    glBindVertexArray(0);
}

Mesh::~Mesh() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Mesh::SetupBuffers(const float* vertices, size_t vertSize, const unsigned int* indices, size_t idxSize) {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertSize, vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, idxSize, indices, GL_STATIC_DRAW);

    // Korrigiertes Stride für alle Attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void Mesh::LoadTextures(const char* texturePath1, const char* texturePath2) {
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (texturePath1) texture1 = ResourceManager::GetTexture(texturePath1);
    if (texturePath2) texture2 = ResourceManager::GetTexture(texturePath2);
}

void Mesh::SetModelMatrices(const std::vector<glm::mat4>& matrices) {
    instanceCount = matrices.size();
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, instanceCount * sizeof(glm::mat4), matrices.data(), GL_DYNAMIC_DRAW);
}

void Mesh::DrawInstanced() const {
    glDrawElementsInstanced(GL_TRIANGLES, static_cast<GLsizei>(indexCount), GL_UNSIGNED_INT, 0, static_cast<GLsizei>(instanceCount));
}

void Mesh::Bind() const {
    glBindVertexArray(VAO);
    if (texture1) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
    }
    if (texture2) {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
    }
}
void Mesh::Unbind() const { glBindVertexArray(0); }
void Mesh::Draw() const { glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indexCount), GL_UNSIGNED_INT, 0); }

void Mesh::SetTexture(const char *texturePath) {
    texture1 = ResourceManager::GetTexture(texturePath);
}
