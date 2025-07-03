#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cmath>

#include "../inlcude/Window.hpp"
#include "../inlcude/Shader.hpp"
#include "../inlcude/Mesh.hpp"
#include "../inlcude/Scene.hpp"
#include "../inlcude/Render.hpp"

//For resizing the viewport when the window size changes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

int main() {

    Window window(1280, 720, "3D Renderer");

    glfwSetFramebufferSizeCallback(window.getGLFWwindow(), framebuffer_size_callback);

    Shader shader("shaders/firstVert.vert", "shaders/firstFrag.frag");
    Scene scene;

    Render renderer(window, scene, shader);
    renderer.run();

    return 0;
}
