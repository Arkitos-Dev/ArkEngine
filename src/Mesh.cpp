//
// Created by Anton on 03.07.2025.
//
#include "../include/Mesh.hpp"
#include <glad/glad.h>
#include <stb_image.h>

Mesh::Mesh(const float* vertices, size_t vertSize, const unsigned int* indices, size_t idxSize, const char* texturePath1, const char* texturePath2)
        : indexCount(idxSize / sizeof(unsigned int)), texture1(0), texture2(0)
{
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

    if (texturePath1) {
        glGenTextures(1, &texture1);
        glBindTexture(GL_TEXTURE_2D, texture1);
        // Parameter setzen ...
        int w, h, c;
        unsigned char* data = stbi_load(texturePath1, &w, &h, &c, 0);
        if (data) {
            GLenum format = (c == 4) ? GL_RGBA : GL_RGB;
            glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        stbi_image_free(data);
    }
    if (texturePath2) {
        glGenTextures(1, &texture2);
        glBindTexture(GL_TEXTURE_2D, texture2);
        // Parameter setzen ...
        int w, h, c;
        unsigned char* data = stbi_load(texturePath2, &w, &h, &c, 0);
        if (data) {
            GLenum format = (c == 4) ? GL_RGBA : GL_RGB;
            glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        stbi_image_free(data);
    }
    glBindVertexArray(0);

}

Mesh::~Mesh() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    if (texture1) glDeleteTextures(1, &texture1);
    if (texture2) glDeleteTextures(1, &texture2);
}

void Mesh::bind() const {
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
void Mesh::unbind() const { glBindVertexArray(0); }
void Mesh::draw() const { glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indexCount), GL_UNSIGNED_INT, 0); }
