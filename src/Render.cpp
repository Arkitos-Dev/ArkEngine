//
// Created by Anton on 03.07.2025.
//
#include "../inlcude/Render.hpp"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <sstream>
#include <thread>

Render::Render(Window& win, Scene& sc, Shader& sh)
        : window(win), scene(sc), shader(sh) {}

void Render::processInput() {
    if (glfwGetKey(window.getGLFWwindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window.getGLFWwindow(), true);
}

void Render::updateFPS() {
    double currentTime = glfwGetTime();
    nbFrames++;
    if (currentTime - lastTime >= 1.0) {
        std::stringstream ss;
        ss << "3D Renderer - FPS: " << nbFrames;
        glfwSetWindowTitle(window.getGLFWwindow(), ss.str().c_str());
        nbFrames = 0;
        lastTime += 1.0;
    }
}

void Render::limitFrameRate(double frameStart, double targetFPS) {
    double frameEnd = glfwGetTime();
    double frameDuration = frameEnd - frameStart;
    double targetFrameTime = 1.0 / targetFPS;
    if (frameDuration < targetFrameTime) {
        std::this_thread::sleep_for(
                std::chrono::duration<double>(targetFrameTime - frameDuration)
        );
    }
}

void Render::run() {
    lastTime = glfwGetTime();
    nbFrames = 0;
    while (!window.shouldClose()) {
        double frameStart = glfwGetTime();

        updateFPS();
        processInput();



        glClearColor(0.4f, 0.0f, 0.6f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        int width, height;
        glfwGetFramebufferSize(window.getGLFWwindow(), &width, &height);
        float aspect = static_cast<float>(width) / static_cast<float>(height);
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);

        glm::vec3 color = {1.0f, 0.5f, 0.0f};
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

        limitFrameRate(frameStart, 120);
    }
}
