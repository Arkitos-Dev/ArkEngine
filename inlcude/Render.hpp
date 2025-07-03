//
// Created by Anton on 03.07.2025.
//

#ifndef INC_3DRENDERER_RENDER_HPP
#define INC_3DRENDERER_RENDER_HPP

#include "Window.hpp"
#include "Shader.hpp"
#include "Scene.hpp"
#include "Cube.hpp"

class Render {
public:
    Render(Window& window, Scene& scene, Shader& shader);
    void run();
private:
    double lastTime = 0.0;
    int nbFrames = 0;
    void updateFPS();
    void limitFrameRate(double frameStart, double targetFPS);
    Window& window;
    Scene& scene;
    Shader& shader;
    void processInput();
};

#endif //INC_3DRENDERER_RENDER_HPP
