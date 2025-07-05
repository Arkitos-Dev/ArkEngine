//
// Created by Anton on 05.07.2025.
//

#include "../../include/core/ArkEngine.hpp"

ArkEngine::ArkEngine(int width, int height, const std::string& title) {}

ArkEngine::~ArkEngine() {}

void ArkEngine::Run() {

    Window window (1920, 1080, "ArkEngine");

    Camera camera(window.GetWindow(), true);

    Shader* shader = ResourceManager::GetShader("shaders/firstVert.vert", "shaders/firstFrag.frag");

    Level level;
    level.LoadLevel(level, "level.bin");

    Scene scene;
    scene.SetLevel(level);

    UI ui(window.GetWindow());

    Renderer renderer(window, scene, shader, camera, level, ui);
    renderer.Render();
}