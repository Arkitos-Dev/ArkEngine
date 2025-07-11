//
// Created by Anton on 01.07.2025.
//
#include "glad/glad.h"
#include "../../include/core/Window.hpp"
#include <iostream>
#include <vector>

std::vector<std::string> droppedFiles;

void drop_callback(GLFWwindow* window, int count, const char** paths) {
    droppedFiles.clear();
    for (int i = 0; i < count; ++i) {
        droppedFiles.push_back(paths[i]);
    }
}

Window::Window(int width, int height, const char* title) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_ALPHA_BITS, 0);
    glfwWindowHint(GLFW_DECORATED, GLFW_TRUE); // Rahmen/Titelleiste

    window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(window);

    // Fenster maximieren (Taskleiste bleibt sichtbar)
    glfwMaximizeWindow(window);

    glfwSetDropCallback(window, drop_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(-1);
    }

    glfwSwapInterval(0);
    glfwSetFramebufferSizeCallback(window, Window::framebuffer_size_callback);
}


Window::~Window() {
    glfwTerminate();
}

bool Window::ShouldClose() const {
    return glfwWindowShouldClose(window);
}

void Window::SwapBuffers() {
    glfwSwapBuffers(window);
}

void Window::PollEvents() {
    glfwPollEvents();
}

GLFWwindow* Window::GetWindow() const {
    return window;
}

void Window::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}
