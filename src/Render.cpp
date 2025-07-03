//
// Created by Anton on 03.07.2025.
//

#include "../inlcude/Render.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>

Render::Render(Window& win, Scene& sc, Shader& sh)
        : window(win), scene(sc), shader(sh) {}

void Render::processInput() {
    if (glfwGetKey(window.getGLFWwindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window.getGLFWwindow(), true);
}

void Render::run() {
    while (!window.shouldClose()) {
        processInput();

        glClearColor(0.4f, 0.0f, 0.6f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        float timeValue = glfwGetTime();
        float greenValue = (sin(timeValue) / 2.0f) + 0.5f;

        shader.use();
        shader.setFloat("someGreenValue", greenValue);

        for (auto mesh : scene.getMeshes()) {
            mesh->bind();
            mesh->draw();
        }

        window.swapBuffers();
        window.pollEvents();
    }
}
