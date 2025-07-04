#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <cmath>

#include "../include/Window.hpp"
#include "../include/Shader.hpp"
#include "../include/Mesh.hpp"
#include "../include/Scene.hpp"
#include "../include/Renderer.hpp"
#include "../include/Cube.hpp"
#include "../include/plane.hpp"
#include <vector>

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    auto* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));
    if (camera)
        camera->mouse_callback(window, xpos, ypos);
}

int main() {
    Window window(1280, 720, "3D Renderer");
    glfwSetFramebufferSizeCallback(window.getGLFWwindow(), framebuffer_size_callback);

    Camera camera(window.getGLFWwindow(), true);

    glfwSetWindowUserPointer(window.getGLFWwindow(), &camera);
    glfwSetCursorPosCallback(window.getGLFWwindow(), mouse_callback);

    Shader shader("shaders/firstVert.vert", "shaders/firstFrag.frag");

    Scene scene;

    glm::vec3 cubePositions[] = {
            { 0.0f,  0.0f,  0.0f},
            { 2.0f,  5.0f, -15.0f},
            {-1.5f, -2.2f, -2.5f},
            {-3.8f, -2.0f, -12.3f},
            { 2.4f, -0.4f, -3.5f},
            {-1.7f,  3.0f, -7.5f},
            { 1.3f, -2.0f, -2.5f},
            { 1.5f,  2.0f, -2.5f},
            { 1.5f,  0.2f, -1.5f},
            {-1.3f,  1.0f, -1.5f}
    };

    for (int i = 0; i < 10; i++) {
        auto* cube = new Cube();
        cube->setPosition(cubePositions[i]);
        float angle = 20.0f * i;
        cube->setRotation(angle, glm::vec3(1.0f, 0.3f, 0.5f));
        scene.addMesh(cube);
    }

    // Erzeuge ein 3x3 Raster aus Planes als Boden
    float planeSize = 1.0f;
    int gridSize = 5;
    for (int x = -gridSize; x <= gridSize; ++x) {
        for (int z = -gridSize; z <= gridSize; ++z) {
            auto* ground = new Plane(planeSize, "resources/images/container.jpg");
            ground->setPosition(glm::vec3(x * planeSize, -1.0f, z * planeSize));
            scene.addMesh(ground);
        }
    }

    Renderer renderer(window, scene, shader, camera);
    renderer.render();

    return 0;
}