#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "GLFW/glfw3.h"

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

    float fov = 45.0f;
    float nearPlane = 0.1f;
    float farPlane = 100.0f;

    Camera(GLFWwindow* window, bool hideCursor = true);
    void UpdateViewMatrix();
    glm::mat4 GetViewMatrix() const;
    glm::mat4 GetProjectionMatrix(float aspect) const;
    float GetNear() const { return nearPlane; }
    float GetFar()  const { return farPlane;  }
    void Movement(GLFWwindow* window, float deltaTime);
    static void mouse_callback_dispatch(GLFWwindow* window, double xpos, double ypos);
    void mouse_callback(GLFWwindow* window, double xpos, double ypos);
};