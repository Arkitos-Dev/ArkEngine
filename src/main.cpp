#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include "Window.hpp"
#include <fstream>
#include <sstream>
#include <string>

std::string loadShaderSource(const char* filePath) {
    std::ifstream file(filePath);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

//For resizing the viewport when the window size changes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main() {

   Window window(1280, 720, "3D Renderer");

    glfwSetFramebufferSizeCallback(window.getGLFWwindow(), framebuffer_size_callback);

    float vertices[] = {
            // Position         // Farbe
            0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f, // unten rechts (rot)
            -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, // unten links (grün)
            0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f, // oben rechts (blau)
            -0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 0.0f  // oben links (gelb)
    };

    unsigned int indices[] = {
            0, 1, 2, // erstes Dreieck
            1, 3, 2  // zweites Dreieck
    };

    std::string vertexCode = loadShaderSource("shaders/firstVert.vert");
    std::string fragmentCode = loadShaderSource("shaders/firstFrag.frag");
    const char* vertexShaderSource = vertexCode.c_str();
    const char* fragmentShaderSource = fragmentCode.c_str();

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    glBindVertexArray(0);

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    int  success;
    char infoLog[512];

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    while(!window.shouldClose())
    {
        processInput(window.getGLFWwindow());

        glClearColor(0.4f, 0.0f, 0.6f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        float timeValue = glfwGetTime();
        float greenValue = (sin(timeValue) / 2.0f) + 0.5f;

        glBindVertexArray(VAO);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        window.swapBuffers();
        window.pollEvents();
    }
    return 0;
}
