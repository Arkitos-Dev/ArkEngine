//
// Created by Anton on 03.07.2025.
//


#include "../inlcude/Render.hpp"
#include <GLFW/glfw3.h>
#include "../inlcude/Render.hpp"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>
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
        glm::vec3 color = {1.0f, 0.5f, 0.0f};

        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1280.0f/720.0f, 0.1f, 100.0f);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
        float angle = glfwGetTime();
        glm::mat4 model = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f));

        shader.use();
        shader.setVec3("uniColor", color);
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);
        shader.setMat4("model", model);

        for (const auto& mesh : scene.getMeshes()) {
            mesh->bind();
            mesh->draw();
            mesh->unbind();
        }

        window.swapBuffers();
        window.pollEvents();
    }
}
