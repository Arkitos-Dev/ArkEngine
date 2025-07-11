#include "../../include/core/UI.hpp"
#include "../../include/objects/Cube.hpp"
#include "../../include/objects/Plane.hpp"
#include "glm/gtx/quaternion.hpp"
#include "ImGuiFileDialog.h"

std::filesystem::path UI::selectedDir;
std::string UI::selectedFile;

UI::UI(Window* windowObj, GLFWwindow* window) : windowObj(windowObj), window(window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    ImGuiIO& io = ImGui::GetIO();
    io.FontDefault = io.Fonts->AddFontFromFileTTF("resources/fonts/DMSans_36pt-Regular.ttf", 16.0f);
    SetStyle();
}

UI::~UI() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void UI::SetStyle() {
    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();

    LoadStyle("style.txt");
}

void UI::BeginFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGuiWindowFlags window_flags = 0;
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    window_flags |= ImGuiWindowFlags_NoTitleBar;

    ImGui::Begin("DockSpace", nullptr, window_flags);
    ImGui::PopStyleVar();
    ImGui::PopStyleVar(2);

    ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), 0);

    ImGui::End();
}

void UI::EndFrame() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UI::Draw(const std::vector<Mesh*>& meshes, Scene& scene) {
    static int selectedIndex = 0;

    // Menüleiste größer machen
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec2 oldPadding = style.FramePadding;
    style.FramePadding.y = 8.0f; // z.B. 8.0f für mehr Höhe

    DrawMainMenu(scene);

    style.FramePadding = oldPadding; // Padding zurücksetzen

    DrawSceneList(scene, selectedIndex);
    DrawObjectInfo(scene, selectedIndex, meshes);
    DrawFileBrowser(scene);
    DrawDirectoryTree();
    DrawStylingEditor();
}

void UI::DrawMainMenu(Scene& scene) {
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec2 oldPadding = style.FramePadding;
    style.FramePadding.y = 10.0f; // Menüleiste höher machen

    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            ImGui::MenuItem("Save");
            ImGui::MenuItem("Load");
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit")) {
            ImGui::MenuItem("Undo", "Strg+Z");
            ImGui::MenuItem("Redo", "Strg+Y");
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    style.FramePadding = oldPadding;
}

void UI::DrawSceneList(Scene& scene, int& selectedIndex) {
    ImGui::Begin("Scene");
    ImGui::Text("Objects:");
    auto& objects = scene.GetObjects();
    std::map<std::string, int> typeCounter;
    bool anyItemHovered = false;

    for (int i = 0; i < objects.size(); ++i) {
        std::string label;
        if (dynamic_cast<Cube*>(objects[i].get()))
            label = "Cube";
        else if (dynamic_cast<Plane*>(objects[i].get()))
            label = "Plane";
        else if (dynamic_cast<Light*>(objects[i].get()))
            label = "Light";
        else
            label = "Unknown";
        int num = ++typeCounter[label];
        label += " " + std::to_string(num);
        bool selected = (selectedIndex == i);
        if (ImGui::Selectable(label.c_str(), selected)) {
            selectedIndex = i;
        }
        if (ImGui::IsItemHovered())
            anyItemHovered = true;

        if (ImGui::BeginPopupContextItem()) {
            if (ImGui::MenuItem("Delete GameObject")) {
                scene.RemoveObjectAt(i);
                if (selectedIndex >= scene.GetObjects().size())
                    selectedIndex = static_cast<int>(scene.GetObjects().size()) - 1;
                ImGui::CloseCurrentPopup();
                ImGui::EndPopup();
                break;
            }
            ImGui::EndPopup();
        }
    }

    // Kontextmenü zum Hinzufügen neuer Objekte
    if (!anyItemHovered && ImGui::BeginPopupContextWindow("SceneContextMenu", ImGuiPopupFlags_MouseButtonRight)) {
        if (ImGui::BeginMenu("Add GameObject")) {
            if (ImGui::MenuItem("Cube")) {
                scene.AddObject(std::make_unique<Cube>());
                selectedIndex = static_cast<int>(scene.GetObjects().size()) - 1;
            }
            if (ImGui::MenuItem("Plane")) {
                scene.AddObject(std::make_unique<Plane>());
                selectedIndex = static_cast<int>(scene.GetObjects().size()) - 1;
            }
            if (ImGui::MenuItem("PointLight")) {
                scene.AddObject(std::make_unique<PointLight>());
                selectedIndex = static_cast<int>(scene.GetObjects().size()) - 1;
            }
            if (ImGui::MenuItem("Directional Light")) {
                scene.AddObject(std::make_unique<DirectionalLight>());
                selectedIndex = static_cast<int>(scene.GetObjects().size()) - 1;
            }
            ImGui::EndMenu();
        }
        ImGui::EndPopup();
    }

    if (ImGui::BeginDragDropTarget()) {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("MODEL_PATH")) {
            const char* modelPath = (const char*)payload->Data;
            auto modelObject = std::make_shared<Model>(modelPath);
            scene.AddObject(modelObject);
            selectedIndex = static_cast<int>(scene.GetObjects().size()) - 1;
        }
        ImGui::EndDragDropTarget();
    }
    ImGui::End();
}

void UI::DrawObjectInfo(Scene& scene, int selectedIndex, const std::vector<Mesh*>& meshes) {
    auto& objects = scene.GetObjects();
    ImGui::Begin("Inspector");
    ImGui::Separator();
    ImGui::Text("Transform");
    if (!objects.empty() && selectedIndex >= 0 && selectedIndex < objects.size()) {
        auto& obj = objects[selectedIndex];

        glm::vec3 pos = obj->GetPosition();
        if (ImGui::DragFloat3("Position", &pos.x, 0.05f))
            obj->SetPosition(pos);

        glm::quat rot = obj->GetRotation();
        float quat[4] = { rot.w, rot.x, rot.y, rot.z };
        if (ImGui::DragFloat4("Rotation", quat, 0.01f)) {
            glm::quat newRot(quat[0], quat[1], quat[2], quat[3]);
            if (glm::length(newRot) > 1e-4f)
                obj->SetRotation(glm::normalize(newRot));
        }

        glm::vec3 scale = obj->GetScale();
        if (ImGui::DragFloat3("Scale", &scale.x, 0.05f, 0.01f, 100.0f))
            obj->SetScale(scale);

        // Licht-Inspector
        if (auto* light = dynamic_cast<Light*>(obj.get())) {
            ImGui::Separator();
            ImGui::Text("Light Optionen");
            ImGui::ColorEdit3("Color", (float*)&light->color);

            if (light->type == Light::Type::Directional) {
                ImGui::DragFloat3("Direction", (float*)&light->direction, 0.01f, -1.0f, 1.0f);
            }
            if (light->type == Light::Type::Point || light->type == Light::Type::Spot) {
                ImGui::DragFloat("Constant", &light->constant, 0.01f, 0.0f, 2.0f);
                ImGui::DragFloat("Linear", &light->linear, 0.01f, 0.0f, 1.0f);
                ImGui::DragFloat("Quadratic", &light->quadratic, 0.01f, 0.0f, 1.0f);
            }
            if (light->type == Light::Type::Spot) {
                ImGui::DragFloat("CutOff", &light->cutOff, 0.01f, 0.0f, 1.0f);
                ImGui::DragFloat("OuterCutOff", &light->outerCutOff, 0.01f, 0.0f, 1.0f);
            }
        }
    } else {
        ImGui::Text("Kein Objekt vorhanden.");
    }
    ImGui::End();
}

// C++
ImVec2 UI::DrawViewport(GLuint texture, int texWidth, int texHeight, Scene& scene) {
    ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    ImVec2 avail = ImGui::GetContentRegionAvail();
    float availAspect = avail.x / avail.y;
    float texAspect = static_cast<float>(texWidth) / texHeight;

    ImVec2 imageSize;
    if (availAspect > texAspect) {
        imageSize.y = avail.y;
        imageSize.x = avail.y * texAspect;
    } else {
        imageSize.x = avail.x;
        imageSize.y = avail.x / texAspect;
    }

    ImVec2 cursor = ImGui::GetCursorPos();
    ImGui::SetCursorPos(ImVec2(
            cursor.x + (avail.x - imageSize.x) * 0.5f,
            cursor.y + (avail.y - imageSize.y) * 0.5f
    ));

    if (ImGui::BeginDragDropTarget()) {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("MODEL_PATH")) {
            const char* modelPath = (const char*)payload->Data;
            // Modell ins Projekt importieren
            ProjectManager::Instance().ImportAsset(modelPath, "model");
            std::string assetPath = ProjectManager::Instance().GetProjectRoot() + "/assets/models/" + std::filesystem::path(modelPath).filename().string();
            auto modelObject = ResourceManager::GetModel(assetPath);
            scene.AddObject(modelObject);
        }
        ImGui::EndDragDropTarget();
    }

    ImGui::Image((void*)(intptr_t)texture, imageSize, ImVec2(0,1), ImVec2(1,0));
    ImGui::End();
    return imageSize;
}

void UI::DrawFileBrowser(Scene& scene) {
    ImGui::Begin("Content");
    if (std::filesystem::is_directory(selectedDir)) {
        for (const auto& entry : std::filesystem::directory_iterator(selectedDir)) {
            if (!entry.is_directory()) {
                if (ImGui::Selectable(entry.path().filename().string().c_str())) {
                    UI::selectedFile = entry.path().string();
                }
                // Drag & Drop für .obj-Dateien
                if (entry.path().extension() == ".obj" && ImGui::BeginDragDropSource()) {
                    ImGui::SetDragDropPayload("MODEL_PATH", entry.path().string().c_str(), entry.path().string().size() + 1);
                    ImGui::Text("Model: %s", entry.path().filename().string().c_str());
                    ImGui::EndDragDropSource();
                }
            }
        }
        extern std::vector<std::string> droppedFiles;

        // Drag & Drop aus Explorer
        if (!droppedFiles.empty()) {
            for (const auto& filePath : droppedFiles) {
                if (std::filesystem::path(filePath).extension() == ".obj") {
                    ProjectManager::Instance().ImportAsset(filePath, "model");
                    selectedDir = selectedDir;
                    // Modell als GameObject zur Szene hinzufügen
                    std::string assetPath = ProjectManager::Instance().GetProjectRoot() + "/assets/models/" + std::filesystem::path(filePath).filename().string();
                    auto modelObject = ResourceManager::GetModel(assetPath);
                    scene.AddObject(modelObject);
                }
            }
            droppedFiles.clear();
        }
        if (ImGui::BeginDragDropTarget()) {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("MODEL_PATH")) {
                const char* modelPath = (const char*)payload->Data;
                ProjectManager::Instance().ImportAsset(modelPath, "model");
                std::string assetPath = ProjectManager::Instance().GetProjectRoot() + "/assets/models/" + std::filesystem::path(modelPath).filename().string();
                auto modelObject = ResourceManager::GetModel(assetPath);
                scene.AddObject(modelObject);
            }
            ImGui::EndDragDropTarget();
        }
    }
    ImGui::End();
}

static std::filesystem::path selectedDir = ProjectManager::Instance().GetProjectRoot();

void UI::DrawDirectoryTree() {
    static std::filesystem::path assetsRoot = ProjectManager::Instance().GetProjectRoot() + "/assets";
    ImGui::Begin("Directory");
    ImGuiTreeNodeFlags rootFlags = (selectedDir == assetsRoot) ? ImGuiTreeNodeFlags_Selected : ImGuiTreeNodeFlags_DefaultOpen;
    bool open = ImGui::TreeNodeEx("assets", rootFlags);
    if (ImGui::IsItemClicked()) {
        selectedDir = assetsRoot;
    }
    if (open) {
        DrawDirectoryTreeRecursive(assetsRoot);
        ImGui::TreePop();
    }
    ImGui::End();
}

void UI::DrawDirectoryTreeRecursive(const std::filesystem::path& dir) {
    for (const auto& entry : std::filesystem::directory_iterator(dir)) {
        if (entry.is_directory()) {
            ImGuiTreeNodeFlags flags = (selectedDir == entry.path()) ? ImGuiTreeNodeFlags_Selected : 0;
            bool open = ImGui::TreeNodeEx(entry.path().filename().string().c_str(), flags);
            if (ImGui::IsItemClicked()) {
                selectedDir = entry.path();
            }
            if (open) {
                DrawDirectoryTreeRecursive(entry.path());
                ImGui::TreePop();
            }
        }
    }
}

void UI::DrawStylingEditor() {
    ImGui::Begin("Styling");

    if (ImGui::Button("Style speichern")) {
        SaveStyle("style.txt");
    }
    ImGui::SameLine();
    if (ImGui::Button("Style laden")) {
        LoadStyle("style.txt");
    }

    ImGuiStyle& style = ImGui::GetStyle();

    ImGui::DragFloat("Alpha", &style.Alpha, 0.01f, 0.2f, 1.0f, "%.2f");
    ImGui::DragFloat2("WindowPadding", (float*)&style.WindowPadding, 0.1f, 0.0f, 30.0f, "%.1f");
    ImGui::DragFloat2("FramePadding", (float*)&style.FramePadding, 0.1f, 0.0f, 20.0f, "%.1f");
    ImGui::DragFloat2("ItemSpacing", (float*)&style.ItemSpacing, 0.1f, 0.0f, 20.0f, "%.1f");
    ImGui::DragFloat2("ItemInnerSpacing", (float*)&style.ItemInnerSpacing, 0.1f, 0.0f, 20.0f, "%.1f");
    ImGui::DragFloat2("TouchExtraPadding", (float*)&style.TouchExtraPadding, 0.1f, 0.0f, 10.0f, "%.1f");
    ImGui::DragFloat("IndentSpacing", &style.IndentSpacing, 0.1f, 0.0f, 30.0f, "%.1f");
    ImGui::DragFloat("ScrollbarSize", &style.ScrollbarSize, 0.1f, 5.0f, 30.0f, "%.1f");
    ImGui::DragFloat("GrabMinSize", &style.GrabMinSize, 0.1f, 1.0f, 20.0f, "%.1f");
    ImGui::DragFloat("WindowBorderSize", &style.WindowBorderSize, 0.01f, 0.0f, 5.0f, "%.2f");
    ImGui::DragFloat("ChildBorderSize", &style.ChildBorderSize, 0.01f, 0.0f, 5.0f, "%.2f");
    ImGui::DragFloat("PopupBorderSize", &style.PopupBorderSize, 0.01f, 0.0f, 5.0f, "%.2f");
    ImGui::DragFloat("FrameBorderSize", &style.FrameBorderSize, 0.01f, 0.0f, 5.0f, "%.2f");
    ImGui::DragFloat("TabBorderSize", &style.TabBorderSize, 0.01f, 0.0f, 5.0f, "%.2f");
    ImGui::DragFloat("WindowRounding", &style.WindowRounding, 0.1f, 0.0f, 20.0f, "%.1f");
    ImGui::DragFloat("ChildRounding", &style.ChildRounding, 0.1f, 0.0f, 20.0f, "%.1f");
    ImGui::DragFloat("FrameRounding", &style.FrameRounding, 0.1f, 0.0f, 20.0f, "%.1f");
    ImGui::DragFloat("PopupRounding", &style.PopupRounding, 0.1f, 0.0f, 20.0f, "%.1f");
    ImGui::DragFloat("ScrollbarRounding", &style.ScrollbarRounding, 0.1f, 0.0f, 20.0f, "%.1f");
    ImGui::DragFloat("GrabRounding", &style.GrabRounding, 0.1f, 0.0f, 20.0f, "%.1f");
    ImGui::DragFloat("LogSliderDeadzone", &style.LogSliderDeadzone, 0.01f, 0.0f, 10.0f, "%.2f");
    ImGui::DragFloat("TabRounding", &style.TabRounding, 0.1f, 0.0f, 20.0f, "%.1f");

    if (ImGui::CollapsingHeader("Colors", ImGuiTreeNodeFlags_DefaultOpen)) {
        for (int i = 0; i < ImGuiCol_COUNT; ++i) {
            ImGui::ColorEdit4(ImGui::GetStyleColorName(i), (float*)&style.Colors[i]);
        }
    }

    if (ImGui::Button("Reset Style")) {
        ImGui::StyleColorsDark();
        SetStyle();
    }

    ImGui::End();
}

#include <fstream>
#include <sstream>

void UI::SaveStyle(const std::string& filename) {
    ImGuiStyle& style = ImGui::GetStyle();
    std::ofstream file(filename);
    if (!file) return;

    file << style.Alpha << "\n";
    file << style.WindowPadding.x << " " << style.WindowPadding.y << "\n";
    file << style.WindowRounding << "\n";
    file << style.WindowBorderSize << "\n";
    file << style.WindowMinSize.x << " " << style.WindowMinSize.y << "\n";
    file << style.WindowTitleAlign.x << " " << style.WindowTitleAlign.y << "\n";
    file << style.ChildRounding << "\n";
    file << style.ChildBorderSize << "\n";
    file << style.PopupRounding << "\n";
    file << style.PopupBorderSize << "\n";
    file << style.FramePadding.x << " " << style.FramePadding.y << "\n";
    file << style.FrameRounding << "\n";
    file << style.FrameBorderSize << "\n";
    file << style.ItemSpacing.x << " " << style.ItemSpacing.y << "\n";
    file << style.ItemInnerSpacing.x << " " << style.ItemInnerSpacing.y << "\n";
    file << style.TouchExtraPadding.x << " " << style.TouchExtraPadding.y << "\n";
    file << style.IndentSpacing << "\n";
    file << style.ColumnsMinSpacing << "\n";
    file << style.ScrollbarSize << "\n";
    file << style.ScrollbarRounding << "\n";
    file << style.GrabMinSize << "\n";
    file << style.GrabRounding << "\n";
    file << style.LogSliderDeadzone << "\n";
    file << style.TabRounding << "\n";
    file << style.TabBorderSize << "\n";
    // Farben
    for (int i = 0; i < ImGuiCol_COUNT; ++i) {
        ImVec4 c = style.Colors[i];
        file << c.x << " " << c.y << " " << c.z << " " << c.w << "\n";
    }
}

void UI::LoadStyle(const std::string& filename) {
    ImGuiStyle& style = ImGui::GetStyle();
    std::ifstream file(filename);
    if (!file) return;

    file >> style.Alpha;
    file >> style.WindowPadding.x >> style.WindowPadding.y;
    file >> style.WindowRounding;
    file >> style.WindowBorderSize;
    file >> style.WindowMinSize.x >> style.WindowMinSize.y;
    file >> style.WindowTitleAlign.x >> style.WindowTitleAlign.y;
    file >> style.ChildRounding;
    file >> style.ChildBorderSize;
    file >> style.PopupRounding;
    file >> style.PopupBorderSize;
    file >> style.FramePadding.x >> style.FramePadding.y;
    file >> style.FrameRounding;
    file >> style.FrameBorderSize;
    file >> style.ItemSpacing.x >> style.ItemSpacing.y;
    file >> style.ItemInnerSpacing.x >> style.ItemInnerSpacing.y;
    file >> style.TouchExtraPadding.x >> style.TouchExtraPadding.y;
    file >> style.IndentSpacing;
    file >> style.ColumnsMinSpacing;
    file >> style.ScrollbarSize;
    file >> style.ScrollbarRounding;
    file >> style.GrabMinSize;
    file >> style.GrabRounding;
    file >> style.LogSliderDeadzone;
    file >> style.TabRounding;
    file >> style.TabBorderSize;
    // Farben
    for (int i = 0; i < ImGuiCol_COUNT; ++i) {
        ImVec4& c = style.Colors[i];
        file >> c.x >> c.y >> c.z >> c.w;
    }
}
