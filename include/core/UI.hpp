// C++
#pragma once
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "GLFW/glfw3.h"
#include <vector>
#include "../objects/Mesh.hpp"
#include "../objects/Level.hpp"
#include <map>
#include "Scene.hpp"
#include "ResourceManager.hpp"

class UI {
public:
    UI(GLFWwindow* window);
    ~UI();

    void BeginFrame();
    void EndFrame();
    void Draw(const std::vector<Mesh*>& meshes, Level& level, Scene& scene);
    ImVec2 DrawViewport(GLuint texture, int texWidth, int texHeight);
    ImVec2 GetRecommendedViewportSize(int texWidth, int texHeight);

    static const char* TypeToString(LevelObject::Type type);
private:
    void DrawMainMenu(Level& level, Scene& scene);
    void DrawSceneList(Level& level, Scene& scene, int& selectedIndex);
    void DrawObjectInfo(Level& level, Scene& scene, int selectedIndex, const std::vector<Mesh*>& meshes);
};