#pragma once
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "Mesh.hpp"
#include "Level.hpp"
#include <map>
#include "Scene.hpp"

class UI {
public:
    UI(GLFWwindow* window);
    ~UI();

    void beginFrame();
    void endFrame();
    void draw(const std::vector<Mesh*>& meshes, Level& level, Scene& scene);
    ImVec2 drawViewport(GLuint texture, int texWidth, int texHeight);
    ImVec2 getRecommendedViewportSize(int texWidth, int texHeight);

    static const char* typeToString(LevelObject::Type type);
private:
    // ggf. Membervariablen, falls benötigt
};