#pragma once
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "GLFW/glfw3.h"
#include <vector>
#include <map>
#include "Scene.hpp"
#include "../objects/Mesh.hpp"
#include "../objects/PointLight.hpp"
#include "../objects/DirectionalLight.hpp"
#include "../core/Window.hpp"
#include "../core/ProjectManager.hpp"

class UI {
public:
    UI(Window* windowObj, GLFWwindow* window);
    ~UI();

    void BeginFrame();
    void EndFrame();
    void Draw(const std::vector<Mesh*>& meshes, Scene& scene);
    ImVec2 DrawViewport(GLuint texture, int texWidth, int texHeight);

    void DrawDirectoryTree();
    void DrawFileBrowser();

    static std::string selectedFile;
    static bool showFileDialog;
    static std::filesystem::path selectedDir;

private:
    void DrawMainMenu(Scene& scene);
    void DrawSceneList(Scene& scene, int& selectedIndex);
    void DrawObjectInfo(Scene& scene, int selectedIndex, const std::vector<Mesh*>& meshes);
    void DrawStylingEditor();
    void SetStyle();
    void LoadStyle(const std::string& filename);
    void SaveStyle(const std::string& filename);
    void DrawDirectoryTreeRecursive(const std::filesystem::path& dir);

    GLFWwindow* window;
    Window* windowObj;
};