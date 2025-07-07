//
// Created by Anton on 03.07.2025.
//

#ifndef INC_3DRENDERER_RENDERER_HPP
#define INC_3DRENDERER_RENDERER_HPP

#include "Window.hpp"
#include "Shader.hpp"
#include "Scene.hpp"
#include "../objects/Cube.hpp"
#include "../../include/objects/Light.hpp"
#include "../../include/objects/PointLight.hpp"
#include "../../include/objects/DirectionalLight.hpp"
#include "Camera.hpp"
#include "UI.hpp"

class Renderer {
public:
    Renderer(Window& window, Scene& scene, Shader* shader, Camera& cam, UI& ui);
    void Render();
private:
    double lastTime = 0.0;
    int nbFrames = 0;
    double deltaTime = 0.0f;
    double lastFrameTime = 0.0f;

    bool paused = false;
    bool escPressedLastFrame = false;

    std::vector<Mesh*> cachedMeshes;
    bool meshesDirty = true;

    GLuint viewportFBO = 0;
    GLuint viewportTexture = 0;
    GLuint viewportRBO = 0;
    int viewportWidth = 1280, viewportHeight = 720;

    unsigned int lightVAO = 0, lightVBO = 0;

    void CreateViewportFBO(int width, int height);
    void DeleteViewportFBO();
    void UpdateFPS();
    void LimitFPS(double frameStart, double targetFPS);
    void SetUpShaders();
    void lightVertices();
    void UpdateMeshCache();
    void RenderMeshes();
    void SetProjectionMatrix(const glm::mat4& projection, const glm::mat4& view);
    void SetMaterials();
    void SetLighting();

    void Input();

    Window& window;
    Scene& scene;
    Shader* shader;
    Camera& camera;
    UI& ui;
};

#endif //INC_3DRENDERER_RENDERER_HPP
