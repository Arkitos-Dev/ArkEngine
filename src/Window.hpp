//
// Created by Anton on 01.07.2025.
//
#pragma once
#include <GLFW/glfw3.h>
#include <iostream>

class Window {
public:
    Window(int width, int height, const char* title);
    ~Window();

    bool shouldClose() const;
    void swapBuffers();
    void pollEvents();
    GLFWwindow* getGLFWwindow() const;

private:
    GLFWwindow* window;
};
