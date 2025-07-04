#include "../include/Window.hpp"
#include "../include/Shader.hpp"
#include "../include/Mesh.hpp"
#include "../include/Scene.hpp"
#include "../include/Renderer.hpp"

int main() {
    Window window(1920, 1080, "3D Renderer");

    Camera camera(window.getGLFWwindow(), true);

    Shader shader("shaders/firstVert.vert", "shaders/firstFrag.frag");

    /*LevelObject levelObjects[] = {
            { LevelObject::Plane, {0,-0.5, 3}, 0.0f, {0,1,0}, {1,1,1} },
            { LevelObject::Plane, {1,-0.5, 3}, 0.0f, {0,1,0}, {1,1,1} },
            { LevelObject::Plane, {-1,-0.5, 3}, 0.0f, {0,1,0}, {1,1,1} },
            { LevelObject::Plane, {0,-0.5, 2}, 0.0f, {0,1,0}, {1,1,1} },
            { LevelObject::Plane, {0,-0.5, 1}, 0.0f, {0,1,0}, {1,1,1} },
            { LevelObject::Plane, {1,-0.5, 2}, 0.0f, {0,1,0}, {1,1,1} },
            { LevelObject::Plane, {-1,-0.5, 2}, 0.0f, {0,1,0}, {1,1,1} },
            { LevelObject::Plane, {0.5,0.0, 1}, 90.0f, {0,0,1}, {1,1,1} },
            { LevelObject::Plane, {-1,0.0,1.5}, 90.0f, {1,0,0}, {1,1,1} },
            { LevelObject::Plane, {-0.5,0.0,1}, 90.0f, {0,0,1}, {1,1,1} },
            { LevelObject::Plane, {-1.5,0.0,2}, 90.0f, {0,0,1}, {1,1,1} },
            { LevelObject::Plane, {-1.5,0.0,3}, 90.0f, {0,0,1}, {1,1,1} },
            { LevelObject::Plane, {-1,0.0,3.5}, 90.0f, {1,0,0}, {1,1,1} },
            { LevelObject::Plane, {0,0.0,3.5}, 90.0f, {1,0,0}, {1,1,1} },
            { LevelObject::Plane, {1,0.0,3.5}, 90.0f, {1,0,0}, {1,1,1} },
            { LevelObject::Plane, {1.5,0.0,2.0}, 90.0f, {0,0,1}, {1,1,1} },
            { LevelObject::Plane, {1.5,0.0,3}, 90.0f, {0,0,1}, {1,1,1} },
            { LevelObject::Plane, {1,0.0,1.5}, 90.0f, {1,0,0}, {1,1,1} },
            { LevelObject::Plane, {0,0.5, 3}, 0.0f, {0,1,0}, {1,1,1} },
            { LevelObject::Plane, {1,0.5, 3}, 0.0f, {0,1,0}, {1,1,1} },
            { LevelObject::Plane, {-1,0.5, 3}, 0.0f, {0,1,0}, {1,1,1} },
            { LevelObject::Plane, {0,0.5, 2}, 0.0f, {0,1,0}, {1,1,1} },
            { LevelObject::Plane, {0,0.5, 1}, 0.0f, {0,1,0}, {1,1,1} },
            { LevelObject::Plane, {1,0.5, 2}, 0.0f, {0,1,0}, {1,1,1} },
            { LevelObject::Plane, {-1,0.5, 2}, 0.0f, {0,1,0}, {1,1,1} }
    };*/

    Level level;
    level.LoadLevel(level, "level.bin");

    /*for (const auto& obj : levelObjects) {
        level.addObject(obj);
    }*/

    Scene scene;
    scene.loadLevel(level);


    Renderer renderer(window, scene, shader, camera, level);
    renderer.render();

    return 0;
}