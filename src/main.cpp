#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// vertex shader source code
const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "void main()\n"
                                 "{\n"
                                 "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                 "}\0";

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

    // Initialize GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    int height = 720;
    int width = 1280;

    // Create a GLFW window
    GLFWwindow* window = glfwCreateWindow(width, height, "3D Renderer", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Set the viewport to the size of the window
    glViewport(0, 0, 1280, 720);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    float vertices[] = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f,  0.5f, 0.0f
    };

    // Generate Buffer ID
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    // Bind the Vertex Buffer Object
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Copy the vertex data into the buffer (Stored now in GPU memory)
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Render loop
    while(!glfwWindowShouldClose(window))
    {
        processInput(window);
        // Swaps the front and back buffers
        glfwSwapBuffers(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Register events
        glfwPollEvents();
    }

    // Clean up and exit
    glfwTerminate();
    return 0;
}
