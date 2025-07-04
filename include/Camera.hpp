//
// Created by Anton on 03.07.2025.
//

#ifndef INC_3DRENDERER_CAMERA_HPP
#define INC_3DRENDERER_CAMERA_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
private:
    glm::vec3 cameraTarget = {0.0f, 0.0f, 0.0f};
    glm::vec3 cameraDirection = glm::normalize(position - cameraTarget);
    glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
    glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);
    bool firstMouse = true;
    float lastX, lastY, yaw, pitch;

public:
    Camera(GLFWwindow* window, bool hideCursor = true) {
        if (hideCursor)
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        // In Camera-Konstruktor nach Position setzen:
        position = glm::vec3(0.0f, 0.0f, 3.0f);
        glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f);
        front = glm::normalize(target - position);

// Yaw und Pitch passend initialisieren:
        yaw = -90.0f; // für Blick entlang -Z, ggf. anpassen
        pitch = 0.0f;

// View-Matrix aktualisieren:
        updateViewMatrix();
    }

    glm::vec3 position = {0.0f, 0.0f, 3.0f};
    glm::vec3 front = {0.0f, 0.0f, 1.0f};
    glm::vec3 up = {0.0f, 1.0f, 0.0f};
    glm::mat4 view = glm::lookAt( position, cameraTarget, up);

    void updateViewMatrix() {
        view = glm::lookAt(position, position + front, up);
    }

    glm::mat4 getViewMatrix() const {return view;}

    void Movement(GLFWwindow* window, float deltaTime) {
        glm::vec3 movement(0.0f);
        const float cameraSpeed = 2.5f * deltaTime;

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            movement += front;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            movement -= front;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            movement -= glm::normalize(glm::cross(front, up));
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            movement += glm::normalize(glm::cross(front, up));
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
            movement += up;
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
            movement -= up;
        if (glm::length(movement) > 0.0f)
            position += glm::normalize(movement) * cameraSpeed;

        updateViewMatrix();
    }

    void mouse_callback(GLFWwindow* window, double xpos, double ypos)
    {
        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos;
        lastX = xpos;
        lastY = ypos;

        float sensitivity = 0.1f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        yaw   += xoffset;
        pitch += yoffset;

        if(pitch > 89.0f)
            pitch = 89.0f;
        if(pitch < -89.0f)
            pitch = -89.0f;

        glm::vec3 direction;
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(direction);

        updateViewMatrix();
    }
};

#endif //INC_3DRENDERER_CAMERA_HPP
