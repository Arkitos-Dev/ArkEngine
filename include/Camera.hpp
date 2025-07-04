#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

class Camera {
private:
    glm::vec3 cameraTarget = {0.0f, 0.0f, 0.0f};
    glm::vec3 cameraDirection;
    glm::vec3 cameraRight;
    glm::vec3 cameraUp;
public:
    float lastX, lastY, yaw, pitch;
    bool firstMouse = true;
    bool* paused = nullptr;

    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::mat4 view;

    Camera(GLFWwindow* window, bool hideCursor = true);
    void updateViewMatrix();
    glm::mat4 getViewMatrix() const;
    void Movement(GLFWwindow* window, float deltaTime);
    static void mouse_callback_dispatch(GLFWwindow* window, double xpos, double ypos);
    void mouse_callback(GLFWwindow* window, double xpos, double ypos);
};