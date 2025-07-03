#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cmath>

#include "../inlcude/Window.hpp"
#include "../inlcude/Shader.hpp"
#include "../inlcude/Mesh.hpp"
#include "../inlcude/Scene.hpp"
#include "../inlcude/Renderer.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

int main() {

    Window window(1280, 720, "3D Renderer");
    glfwSetFramebufferSizeCallback(window.getGLFWwindow(), framebuffer_size_callback);

    Shader shader("shaders/firstVert.vert", "shaders/firstFrag.frag");

    Scene scene;
    scene.addMesh(new Cube());
    scene.addMesh(new Cube());

    scene.getMeshes()[0]->setPosition(glm::vec3(0.5f, 0.5f, 0.0f));
    scene.getMeshes()[1]->setPosition(glm::vec3(-0.5f, -0.5f, 0.0f));


    Renderer renderer(window, scene, shader);
    renderer.render();

    return 0;
}
