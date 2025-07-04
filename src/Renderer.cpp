//
// Created by Anton on 03.07.2025.
//
#include "../include/Renderer.hpp"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <sstream>
#include <thread>

Renderer::Renderer(Window& win, Scene& sc, Shader& sh, Camera& cam)
        : window(win), scene(sc), shader(sh), camera(cam) {
    setUpShaderTextures();
    glEnable(GL_DEPTH_TEST);
    deltaTime = 0.0;
    lastFrameTime = glfwGetTime();
}

void Renderer::Input() {
    if (glfwGetKey(window.getGLFWwindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window.getGLFWwindow(), true);
    camera.Movement(window.getGLFWwindow(), deltaTime);
}

void Renderer::updateFPS() {
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

void Renderer::limitFrameRate(double frameStart, double targetFPS) {
    double frameEnd = glfwGetTime();
    double frameDuration = frameEnd - frameStart;
    double targetFrameTime = 1.0 / targetFPS;
    if (frameDuration < targetFrameTime) {
        std::this_thread::sleep_for(
                std::chrono::duration<double>(targetFrameTime - frameDuration)
        );
    }
}

void Renderer::setUpShaderTextures() {
    shader.use();
    shader.setInt("texture1", 0);
    shader.setInt("texture2", 1);
}

void Renderer::render() {
    while (!window.shouldClose()) {
        double frameStart = glfwGetTime();
        deltaTime = frameStart - lastFrameTime;
        lastFrameTime = frameStart;

        updateFPS();
        Input();

        int width, height;
        glfwGetFramebufferSize(window.getGLFWwindow(), &width, &height);
        float aspect = static_cast<float>(width) / static_cast<float>(height);

        glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);

        glm::vec3 color = {1.0f, 0.5f, 0.0f};

        shader.setVec3("uniColor", color);
        shader.setMat4("projection", projection);
        shader.setMat4("view", camera.getViewMatrix());

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        for (const auto& mesh : scene.getMeshes()) {
            shader.setMat4("model", mesh->getModelMatrix());
            mesh->bind();
            mesh->draw();
            mesh->unbind();
        }

        window.swapBuffers();
        window.pollEvents();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        limitFrameRate(frameStart, 120);
    }
}
