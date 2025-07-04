//
// Created by Anton on 05.07.2025.
//

#include "../include/ArkEngine.hpp"

ArkEngine::ArkEngine(int width, int height, const std::string& title) {}

ArkEngine::~ArkEngine() {}

void ArkEngine::Run() {

    Window window (1920, 1080, "ArkEngine");

    Camera camera(window.getGLFWwindow(), true);

    Shader* shader = ResourceManager::GetShader("shaders/firstVert.vert", "shaders/firstFrag.frag");

    Level level;
    level.LoadLevel(level, "level.bin");

    Scene scene;
    scene.loadLevel(level);

    Renderer renderer(window, scene, shader, camera, level);
    renderer.render();
}
