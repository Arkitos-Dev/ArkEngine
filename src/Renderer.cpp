//
// Created by Anton on 03.07.2025.
//
#include "../include/Renderer.hpp"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <sstream>
#include <thread>
#include <map>

Renderer::Renderer(Window& win, Scene& sc, Shader& sh, Camera& cam, const Level& lvl)
        : window(win), scene(sc), shader(sh), camera(cam), ui(win.getGLFWwindow()), level(lvl) {
    camera.paused = &paused;
    setUpShaderTextures();
    glEnable(GL_DEPTH_TEST);
    deltaTime = 0.0;
    lastFrameTime = glfwGetTime();
}

void Renderer::Input() {
    if (glfwGetKey(window.getGLFWwindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        if (!escPressedLastFrame) {
            paused = !paused;
            if (paused)
                glfwSetInputMode(window.getGLFWwindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            else {
                glfwSetInputMode(window.getGLFWwindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                double xpos, ypos;
                glfwGetCursorPos(window.getGLFWwindow(), &xpos, &ypos);
                camera.lastX = xpos;
                camera.lastY = ypos;
                camera.firstMouse = true; // <-- Hinzufügen!
            }
        }
        escPressedLastFrame = true;
    } else {
        escPressedLastFrame = false;
    }

    // Bewegung und Kamera nur wenn nicht pausiert
    if (!paused) {
        camera.Movement(window.getGLFWwindow(), deltaTime);
    }
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
        ui.beginFrame();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        int width, height;
        glfwGetFramebufferSize(window.getGLFWwindow(), &width, &height);
        float aspect = static_cast<float>(width) / static_cast<float>(height);

        glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);

        shader.setMat4("projection", projection);
        shader.setMat4("view", camera.getViewMatrix());

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        std::map<Mesh*, std::vector<glm::mat4>> meshGroups;
        for (auto* mesh : scene.getMeshes()) {
            meshGroups[mesh->getPrototype()].push_back(mesh->getModelMatrix());
        }

        for (auto& [prototype, matrices] : meshGroups) {
            if (!matrices.empty()) {
                prototype->setInstanceModelMatrices(matrices);
                prototype->bind();
                prototype->drawInstanced();
                prototype->unbind();
            }
        }
        ui.draw(scene.getMeshes(), level);

        ui.endFrame();
        window.swapBuffers();
        window.pollEvents();

        //limitFrameRate(frameStart, 120);
    }
}
