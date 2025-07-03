//
// Created by Anton on 03.07.2025.
//

#ifndef INC_3DRENDERER_RENDERER_HPP
#define INC_3DRENDERER_RENDERER_HPP

#include <glad/glad.h>
#include "Window.hpp"
#include "Shader.hpp"
#include "Scene.hpp"
#include "Cube.hpp"

class Renderer {
public:
    Renderer(Window& window, Scene& scene, Shader& shader);
    void render();
private:
    double lastTime = 0.0;
    int nbFrames = 0;
    void updateFPS();
    void limitFrameRate(double frameStart, double targetFPS);
    void loadTextures();
    void setUpShaderTextures();
    Window& window;
    Scene& scene;
    Shader& shader;
    void processInput();
};

#endif //INC_3DRENDERER_RENDERER_HPP
