//
// Created by Anton on 05.07.2025.
//

#include "../../include/core/ArkEngine.hpp"

ArkEngine::ArkEngine(int width, int height, const std::string& title) {}

ArkEngine::~ArkEngine() {}

void ArkEngine::Run() {

    Window window (1920, 1080, "ArkEngine");

    Camera camera(window.GetWindow(), true);

    auto shader = ResourceManager::GetShader("shaders/StandardLit.vert", "shaders/StandardLit.frag");

    Scene scene;

    auto colonne = std::make_unique<Model>("resources/model/backpack/backpack.obj");
    scene.AddObject(std::unique_ptr<GameObject>(std::move(colonne)));

    UI ui(window.GetWindow());

    Renderer renderer(window, scene, shader, camera, ui);
    renderer.Render();
}