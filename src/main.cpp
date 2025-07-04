#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <cmath>

#include "../include/Window.hpp"
#include "../include/Shader.hpp"
#include "../include/Mesh.hpp"
#include "../include/Scene.hpp"
#include "../include/Renderer.hpp"
#include "../include/plane.hpp"
#include "../include/Level.hpp"

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

    LevelObject levelObjects[] = {
            { LevelObject::Plane, {0,0,0}, 0.0f, {0,1,0}, {10,1,10} },
            { LevelObject::Plane, {0,0,0}, 0.0f, {0,1,0}, {0,0,0} },
            { LevelObject::Cube,  {0,1,0}, 0.0f, {0,1,0}, {1,1,1} },
            { LevelObject::Cube,  {2,1,2}, 45.0f, {0,1,0}, {1,2,1} }
    };

    Level level;
    for (const auto& obj : levelObjects) {
        level.addObject(obj);
    }

    Scene scene;

    scene.loadLevel(level);

    Renderer renderer(window, scene, shader, camera);
    renderer.render();

    return 0;
}