//
// Created by Anton on 03.07.2025.
//

#ifndef INC_3DRENDERER_RENDERER_HPP
#define INC_3DRENDERER_RENDERER_HPP

#include "Window.hpp"
#include "Shader.hpp"
#include "Scene.hpp"
#include "Cube.hpp"
#include "Camera.hpp"
#include "UI.hpp"

class Renderer {
public:
    Renderer(Window& window, Scene& scene, Shader& shader, Camera& cam, Level& lvl);
    void render();
private:
    double lastTime = 0.0;
    int nbFrames = 0;
    double deltaTime = 0.0f;
    double lastFrameTime = 0.0f;

    bool paused = false;
    bool escPressedLastFrame = false;

    GLuint viewportFBO = 0;
    GLuint viewportTexture = 0;
    GLuint viewportRBO = 0;
    int viewportWidth = 1280, viewportHeight = 720;

    void createViewportFBO(int width, int height);
    void deleteViewportFBO();
    void updateFPS();
    void limitFrameRate(double frameStart, double targetFPS);
    void setUpShaderTextures();
    void Input();

    Window& window;
    Scene& scene;
    Shader& shader;
    Camera& camera;
    UI ui;
    Level& level;
};

#endif //INC_3DRENDERER_RENDERER_HPP
