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

    bool ShouldClose() const;
    void SwapBuffers();
    void PollEvents();

    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

    GLFWwindow* GetWindow() const;

private:
    GLFWwindow* window;
};
