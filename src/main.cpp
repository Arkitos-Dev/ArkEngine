#include "../include/Window.hpp"
#include "../include/Shader.hpp"
#include "../include/Mesh.hpp"
#include "../include/Scene.hpp"
#include "../include/Renderer.hpp"
#include "../include/ResourceManager.hpp"

int main() {
    Window window(1920, 1080, "3D Renderer");

    Camera camera(window.getGLFWwindow(), true);

    Shader* shader = ResourceManager::GetShader("shaders/firstVert.vert", "shaders/firstFrag.frag");

    Level level;
    level.LoadLevel(level, "level.bin");

    Scene scene;
    scene.loadLevel(level);

    Renderer renderer(window, scene, shader, camera, level);
    renderer.render();

    return 0;
}