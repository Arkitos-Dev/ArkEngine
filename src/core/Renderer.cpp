//
// Created by Anton on 03.07.2025.
//
#include "glad/glad.h"
#include "../../include/core/Renderer.hpp"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <sstream>
#include <thread>
#include <map>

Renderer::Renderer(Window& win, Scene& sc, Shader* sh, Camera& cam, UI& ui)
        : window(win), scene(sc), shader(sh), camera(cam), ui(ui) {
    camera.paused = &paused;
    SetUpShaders();
    glEnable(GL_DEPTH_TEST);
    deltaTime = 0.0;
    lastFrameTime = glfwGetTime();

    lightVertices();
}

void Renderer::Input() {
    if (glfwGetKey(window.GetWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        if (!escPressedLastFrame) {
            paused = !paused;
            if (paused)
                glfwSetInputMode(window.GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            else {
                glfwSetInputMode(window.GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                double xpos, ypos;
                glfwGetCursorPos(window.GetWindow(), &xpos, &ypos);
                camera.lastX = xpos;
                camera.lastY = ypos;
                camera.firstMouse = true; // <-- Hinzufügen!
            }
        }
        escPressedLastFrame = true;
    } else {
        escPressedLastFrame = false;
    }

    // Bewegung und Kamera nur wenn nicht pausiert
    if (!paused) {
        camera.Movement(window.GetWindow(), deltaTime);
    }
}

void Renderer::UpdateFPS() {
    double currentTime = glfwGetTime();
    nbFrames++;
    if (currentTime - lastTime >= 1.0) {
        std::stringstream ss;
        ss << "3D Renderer - FPS: " << nbFrames;
        glfwSetWindowTitle(window.GetWindow(), ss.str().c_str());
        nbFrames = 0;
        lastTime += 1.0;
    }
}

void Renderer::LimitFPS(double frameStart, double targetFPS) {
    if (targetFPS <= 0.0) return;

    double targetFrameTime = 1.0 / targetFPS;
    double elapsed;

    do {
        elapsed = glfwGetTime() - frameStart;
        if (elapsed < targetFrameTime) {
            std::this_thread::yield();
        }
    } while (elapsed < targetFrameTime);
}

void Renderer::SetUpShaders() {
    shader->Use();
    shader->SetInt("texture1", 0);
    shader->SetInt("texture2", 1);
}

void Renderer::lightVertices() {
    float lightVertices[] = {
        -0.5f,-0.5f,-0.5f,  0.5f,-0.5f,-0.5f,  0.5f, 0.5f,-0.5f,  0.5f, 0.5f,-0.5f, -0.5f, 0.5f,-0.5f, -0.5f,-0.5f,-0.5f,
        -0.5f,-0.5f, 0.5f,  0.5f,-0.5f, 0.5f,  0.5f, 0.5f, 0.5f,  0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f,-0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f, -0.5f, 0.5f,-0.5f, -0.5f,-0.5f,-0.5f, -0.5f,-0.5f,-0.5f, -0.5f,-0.5f, 0.5f, -0.5f, 0.5f, 0.5f,
         0.5f, 0.5f, 0.5f,  0.5f, 0.5f,-0.5f,  0.5f,-0.5f,-0.5f,  0.5f,-0.5f,-0.5f,  0.5f,-0.5f, 0.5f,  0.5f, 0.5f, 0.5f,
        -0.5f,-0.5f,-0.5f,  0.5f,-0.5f,-0.5f,  0.5f,-0.5f, 0.5f,  0.5f,-0.5f, 0.5f, -0.5f,-0.5f, 0.5f, -0.5f,-0.5f,-0.5f,
        -0.5f, 0.5f,-0.5f,  0.5f, 0.5f,-0.5f,  0.5f, 0.5f, 0.5f,  0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f,-0.5f
    };
    glGenVertexArrays(1, &lightVAO);
    glGenBuffers(1, &lightVBO);
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(lightVertices), lightVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}

// Renderer.cpp
void Renderer::CreateViewportFBO(int width, int height) {
    if (viewportFBO) DeleteViewportFBO();

    glGenFramebuffers(1, &viewportFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, viewportFBO);

    glGenTextures(1, &viewportTexture);
    glBindTexture(GL_TEXTURE_2D, viewportTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, viewportTexture, 0);

    glGenRenderbuffers(1, &viewportRBO);
    glBindRenderbuffer(GL_RENDERBUFFER, viewportRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, viewportRBO);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer nicht komplett!" << std::endl;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::DeleteViewportFBO() {
    if (viewportTexture) glDeleteTextures(1, &viewportTexture);
    if (viewportRBO) glDeleteRenderbuffers(1, &viewportRBO);
    if (viewportFBO) glDeleteFramebuffers(1, &viewportFBO);
    viewportTexture = 0;
    viewportRBO = 0;
    viewportFBO = 0;
}

void Renderer::UpdateMeshCache() {
    if (!meshesDirty) return;
    cachedMeshes.clear();
    for (auto& obj : scene.GetObjects()) {
        if (obj.mesh) cachedMeshes.push_back(obj.mesh);
    }
    meshesDirty = false;
}

void Renderer::Render() {
    CreateViewportFBO(viewportWidth, viewportHeight);
    while (!window.ShouldClose()) {
        double frameStart = glfwGetTime();
        deltaTime = frameStart - lastFrameTime;
        lastFrameTime = frameStart;

        UpdateFPS();
        Input();

        glBindFramebuffer(GL_FRAMEBUFFER, viewportFBO);
        glViewport(0, 0, viewportWidth, viewportHeight);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        int width, height;
        glfwGetFramebufferSize(window.GetWindow(), &width, &height);
        float aspect = static_cast<float>(width) / static_cast<float>(height);

        glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);

        glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

        // 1. LightCube rendern
        Shader* lightShader = ResourceManager::GetShader("shaders/lightVert.vert", "shaders/lightFrag.frag");
        lightShader->Use();
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(1.2f, 1.0f, 2.0f));
        model = glm::scale(model, glm::vec3(0.2f));
        lightShader->SetMat4("model", model);
        lightShader->SetMat4("view", camera.GetViewMatrix());
        lightShader->SetMat4("projection", projection);
        lightShader->SetVec3("objectColor", glm::vec3(1.0f, 1.0f, 1.0f));
        lightShader->SetVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        shader->Use();
        shader->SetInt("texture1", 0);
        shader->SetInt("texture2", 1);
        shader->SetMat4("projection", projection);
        shader->SetMat4("view", camera.GetViewMatrix());
        shader->SetVec3("lightPos", lightPos);
        shader->SetVec3("viewPos", camera.position);
        shader->SetVec3("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
        shader->SetVec3("material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
        shader->SetVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
        shader->SetFloat("material.shininess", 32.0f);
        shader->SetVec3("light.ambient",  glm::vec3(0.1f, 0.1f, 0.1f));
        shader->SetVec3("light.diffuse",  glm::vec3(0.5f, 0.0f, 1.0f));
        shader->SetVec3("light.specular", glm::vec3(0.5f, 0.5f, 0.5f));
        shader->SetInt("material.diffuse", 0);

        shader->SetInt("material.diffuse", 0);
        shader->SetInt("material.specular", 1);

        glm::vec3 pointLightPositions[] = {
                glm::vec3( 0.7f,  0.2f,  2.0f),
                glm::vec3( 2.3f, -3.3f, -4.0f),
                glm::vec3(-4.0f,  2.0f, -12.0f),
                glm::vec3( 0.0f,  0.0f, -3.0f)
        };

        for (int i = 0; i < 4; ++i) {
            std::string idx = "pointLights[" + std::to_string(i) + "]";
            shader->SetVec3(idx + ".position", pointLightPositions[i]);
            shader->SetVec3(idx + ".ambient", glm::vec3(0.05f));
            shader->SetVec3(idx + ".diffuse", glm::vec3(0.8f));
            shader->SetVec3(idx + ".specular", glm::vec3(1.0f));
            shader->SetFloat(idx + ".constant", 1.0f);
            shader->SetFloat(idx + ".linear", 0.09f);
            shader->SetFloat(idx + ".quadratic", 0.032f);
        }

        // Directional Light setzen
        shader->SetVec3("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
        shader->SetVec3("dirLight.ambient",  glm::vec3(0.05f, 0.05f, 0.05f));
        shader->SetVec3("dirLight.diffuse",  glm::vec3(0.4f, 0.4f, 0.4f));
        shader->SetVec3("dirLight.specular", glm::vec3(0.5f, 0.5f, 0.5f));

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        std::map<Mesh*, std::vector<glm::mat4>> meshGroups;
        for (auto& sceneObj : scene.GetObjects()) {
            if (sceneObj.mesh) {
                meshGroups[sceneObj.mesh->GetPrototype()].push_back(sceneObj.mesh->GetModelMatrix());
            }
        }

        for (auto& [prototype, matrices] : meshGroups) {
            if (!matrices.empty()) {
                prototype->SetModelMatrices(matrices);
                prototype->Bind();
                prototype->DrawInstanced();
                prototype->Unbind();
            }
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        UpdateMeshCache();

        ui.BeginFrame();
        ui.DrawViewport(viewportTexture, viewportWidth, viewportHeight);
        ui.Draw(cachedMeshes, scene);
        ui.EndFrame();

        window.SwapBuffers();
        window.PollEvents();

        LimitFPS(frameStart, 300.0);
    }
    DeleteViewportFBO();
}
