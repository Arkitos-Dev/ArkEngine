//
// Created by Anton on 01.07.2025.
//
#pragma once
#include "GLFW/glfw3.h"
#include <iostream>

class Window {
public:
    Window(int width, int height, const char* title);
    ~Window();

    bool shouldClose() const;
    void swapBuffers();
    void pollEvents();
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    void setFramebufferSizeCallback();
    void setCursorPosCallback(void* userPointer, void (*callback)(GLFWwindow*, double, double));
    void setUserPointer(void* ptr);
    GLFWwindow* getGLFWwindow() const;

private:
    GLFWwindow* window;
};
