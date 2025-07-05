#pragma once
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "GLFW/glfw3.h"
#include <vector>
#include "../objects/Mesh.hpp"
#include <map>
#include "Scene.hpp"
#include "ResourceManager.hpp"

class UI {
public:
    UI(GLFWwindow* window);
    ~UI();

    void BeginFrame();
    void EndFrame();
    void Draw(const std::vector<Mesh*>& meshes, Scene& scene);
    ImVec2 DrawViewport(GLuint texture, int texWidth, int texHeight);

    static const char* TypeToString(LevelObject::Type type);
private:
    void DrawMainMenu(Scene& scene);
    void DrawSceneList(Scene& scene, int& selectedIndex);
    void DrawObjectInfo(Scene& scene, int selectedIndex, const std::vector<Mesh*>& meshes);
};