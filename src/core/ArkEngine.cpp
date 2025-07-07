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
    level.Load("level.bin");

    Scene scene;
    scene.FromLevel(level);

    SceneObject light1(LevelObject::Cube, glm::vec3(2,2,2), 0, glm::vec3(0,1,0), glm::vec3(0.2f), nullptr);
    light1.kind = SceneObject::ObjectKind::Light;
    light1.lightType = SceneObject::LightType::Point;
    light1.color = glm::vec3(1,0,0);

    SceneObject light2(LevelObject::Cube, glm::vec3(-2,2,0), 0, glm::vec3(0,1,0), glm::vec3(0.2f), nullptr);
    light2.kind = SceneObject::ObjectKind::Light;
    light2.lightType = SceneObject::LightType::Point;
    light2.color = glm::vec3(0,1,0);

    SceneObject light3(LevelObject::Cube, glm::vec3(0,2,-2), 0, glm::vec3(0,1,0), glm::vec3(0.2f), nullptr);
    light3.kind = SceneObject::ObjectKind::Light;
    light3.lightType = SceneObject::LightType::Point;
    light3.color = glm::vec3(0,0,1);

    // Directional Light hinzufügen
    SceneObject dirLight(LevelObject::Cube, glm::vec3(0, 5, 0), 0, glm::vec3(0,1,0), glm::vec3(0.2f), nullptr);
    dirLight.kind = SceneObject::ObjectKind::Light;
    dirLight.lightType = SceneObject::LightType::Directional;
    dirLight.color = glm::vec3(1.0f, 1.0f, 1.0f); // Weißes Licht
    dirLight.direction = glm::vec3(-0.2f, -1.0f, -0.3f); // Richtung von oben schräg

    scene.GetObjects().push_back(dirLight);
    scene.GetObjects().push_back(light1);
    scene.GetObjects().push_back(light2);
    scene.GetObjects().push_back(light3);


    UI ui(window.GetWindow());

    Renderer renderer(window, scene, shader, camera, ui);
    renderer.Render();
}