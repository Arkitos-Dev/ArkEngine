#include "../../include/core/UI.hpp"
#include "../../include/objects/Cube.hpp"
#include "../../include/objects/Plane.hpp"
#include "glm/gtx/quaternion.hpp"
#include "ImGuiFileDialog.h"

std::filesystem::path UI::selectedDir;
std::string UI::selectedFile;
static std::filesystem::path parentDirForNewFolder;
static std::filesystem::path pathToRename;
static char renameName[128] = "";

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

        // Rechtsklick-Kontextmenü für Objekt-Löschung
        if (ImGui::BeginPopupContextItem(("ObjectMenu" + std::to_string(i)).c_str())) {
            if (ImGui::MenuItem("Delete")) {
                scene.RemoveObjectAt(i);
                if (selectedIndex >= static_cast<int>(objects.size())) {
                    selectedIndex = static_cast<int>(objects.size()) - 1;
                }
                if (selectedIndex < 0) selectedIndex = 0;
            }
            ImGui::EndPopup();
        }
    }
    if (ImGui::BeginDragDropTarget()) {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("MODEL_PATH")) {
            const char* modelPath = (const char*)payload->Data;
            std::string pathStr(modelPath);

            std::string projectRoot = ProjectManager::Instance().GetProjectRoot();
            if (pathStr.find(projectRoot + "/assets/") != std::string::npos) {
                auto modelObject = std::make_shared<Model>(pathStr);
                scene.AddObject(modelObject);
            } else {
                auto modelObject = std::make_shared<Model>(pathStr);
                scene.AddObject(modelObject);
            }
            selectedIndex = static_cast<int>(scene.GetObjects().size()) - 1;
        }
        ImGui::EndDragDropTarget();
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
    ImVec2 centeredPos = ImVec2(
            cursor.x + (avail.x - imageSize.x) * 0.5f,
            cursor.y + (avail.y - imageSize.y) * 0.5f
    );
    ImGui::SetCursorPos(centeredPos);

    // Invisible Button über das gesamte Bild für Drag & Drop
    ImGui::InvisibleButton("viewport_dragdrop", imageSize);

    if (ImGui::BeginDragDropTarget()) {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("MODEL_PATH")) {
            const char* modelPath = (const char*)payload->Data;
            std::string pathStr(modelPath);

            // Direkt neue Model-Instanz erstellen, kein Import
            auto modelObject = std::make_shared<Model>(pathStr);
            scene.AddObject(modelObject);
        }
        ImGui::EndDragDropTarget();
    }

    // Texture über dem Button zeichnen
    ImGui::SetCursorPos(centeredPos);
    ImGui::Image((void*)(intptr_t)texture, imageSize, ImVec2(0,1), ImVec2(1,0));

    ImGui::End();
    return imageSize;
}

void UI::DrawFileBrowser(Scene& scene) {
    ImGui::Begin("Content");

    try {
        if (std::filesystem::exists(selectedDir) && std::filesystem::is_directory(selectedDir)) {
            std::error_code ec;
            auto dirIter = std::filesystem::directory_iterator(selectedDir, ec);
            if (ec) {
                ImGui::Text("Fehler beim Öffnen des Verzeichnisses: %s", ec.message().c_str());
                ImGui::End();
                return;
            }

            for (const auto& entry : dirIter) {
                if (!entry.is_directory()) {
                    if (ImGui::Selectable(entry.path().filename().string().c_str())) {
                        UI::selectedFile = entry.path().string();
                    }
                    if (entry.path().extension() == ".obj" && ImGui::BeginDragDropSource()) {
                        std::string fullPath = entry.path().string();
                        ImGui::SetDragDropPayload("MODEL_PATH", fullPath.c_str(), fullPath.size() + 1);
                        ImGui::Text("Model: %s", entry.path().filename().string().c_str());
                        ImGui::EndDragDropSource();
                    }
                }
            }
        } else {
            ImGui::Text("Ungültiges Verzeichnis: %s", selectedDir.string().c_str());
        }
    } catch (const std::filesystem::filesystem_error& e) {
        ImGui::Text("Filesystem-Fehler: %s", e.what());
    }

    extern std::vector<std::string> droppedFiles;

    // Nur externe Dateien importieren (Drag & Drop von außerhalb)
    if (!droppedFiles.empty()) {
        for (const auto& filePath : droppedFiles) {
            if (std::filesystem::path(filePath).extension() == ".obj") {
                std::filesystem::path projectRoot = ProjectManager::Instance().GetProjectRoot();
                std::filesystem::path assetsPath = projectRoot / "assets";
                // Nur importieren wenn externe Datei
                if (filePath.find(assetsPath.string()) == std::string::npos) {
                    ProjectManager::Instance().ImportAsset(filePath, "model");
                }
            }
        }
        droppedFiles.clear();
    }

    ImGui::End();
}

void UI::DrawDirectoryTree() {
    static std::filesystem::path assetsRoot = std::filesystem::path(ProjectManager::Instance().GetProjectRoot()) / "assets";

    // Falls selectedDir noch nicht initialisiert wurde
    if (selectedDir.empty()) {
        selectedDir = assetsRoot;
    }

    ImGui::Begin("Directory");

    // Popup für das Umbenennen (bleibt bestehen)
    if (ImGui::BeginPopup("RenameFolderPopup")) {
        ImGui::Text("Rename Folder");
        ImGui::InputText("##RenameName", renameName, sizeof(renameName));
        if (ImGui::Button("Rename")) {
            std::filesystem::path newPath = pathToRename.parent_path() / renameName;
            if (!std::filesystem::exists(newPath)) {
                std::filesystem::rename(pathToRename, newPath);
                if (selectedDir == pathToRename) {
                    selectedDir = newPath;
                }
            }
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel")) {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }

    // Root-Node
    ImGuiTreeNodeFlags rootFlags = ImGuiTreeNodeFlags_DefaultOpen;
    if (selectedDir == assetsRoot) {
        rootFlags |= ImGuiTreeNodeFlags_Selected;
    }
    bool rootOpen = ImGui::TreeNodeEx("assets", rootFlags);

    if (ImGui::IsItemClicked()) {
        selectedDir = assetsRoot;
    }

    // Kontextmenü für den Root-Ordner
    if (ImGui::BeginPopupContextItem("RootContext")) {
        if (ImGui::MenuItem("New Folder")) {
            std::string folderName = "New Folder";
            int counter = 1;
            std::filesystem::path newPath = assetsRoot / folderName;

            while (std::filesystem::exists(newPath)) {
                newPath = assetsRoot / (folderName + " " + std::to_string(counter));
                counter++;
            }

            bool success = ProjectManager::Instance().CreateFolder(assetsRoot.string(), newPath.filename().string());
            if (!success) {
                std::cerr << "Fehler beim Erstellen des Root-Ordners!" << std::endl;
            }
        }
        ImGui::EndPopup();
    }

    // Kontextmenü für leeren Bereich im Fenster - IMMER im Root-Verzeichnis erstellen
    if (ImGui::BeginPopupContextWindow("DirectoryWindowContext", ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems)) {
        if (ImGui::MenuItem("New Folder")) {
            std::string folderName = "New Folder";
            int counter = 1;
            std::filesystem::path newPath = assetsRoot / folderName; // Immer assetsRoot verwenden

            while (std::filesystem::exists(newPath)) {
                newPath = assetsRoot / (folderName + " " + std::to_string(counter));
                counter++;
            }

            bool success = ProjectManager::Instance().CreateFolder(assetsRoot.string(), newPath.filename().string());
            if (!success) {
                std::cerr << "Fehler beim Erstellen des Root-Ordners!" << std::endl;
            }
        }
        ImGui::EndPopup();
    }

    if (rootOpen) {
        DrawDirectoryTreeRecursive(assetsRoot);
        ImGui::TreePop();
    }

    ImGui::End();
}

void UI::DrawDirectoryTreeRecursive(const std::filesystem::path& dir) {
    try {
        // Prüfen ob das Verzeichnis existiert und zugänglich ist
        if (!std::filesystem::exists(dir) || !std::filesystem::is_directory(dir)) {
            return;
        }

        std::error_code ec;
        auto dirIter = std::filesystem::directory_iterator(dir, ec);
        if (ec) {
            std::cerr << "Fehler beim Öffnen des Verzeichnisses: " << dir << " - " << ec.message() << std::endl;
            return;
        }

        for (const auto& entry : dirIter) {
            if (entry.is_directory()) {
                std::string folderName = entry.path().filename().string();

                ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth;
                if (selectedDir == entry.path()) {
                    flags |= ImGuiTreeNodeFlags_Selected;
                }

                bool nodeOpen = ImGui::TreeNodeEx(folderName.c_str(), flags);

                if (ImGui::IsItemClicked()) {
                    selectedDir = entry.path();
                }

                DrawDirectoryContextMenu(entry.path(), false);

                if (nodeOpen) {
                    DrawDirectoryTreeRecursive(entry.path());
                    ImGui::TreePop();
                }
            }
        }
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Filesystem-Fehler in DrawDirectoryTreeRecursive: " << e.what() << std::endl;
        std::cerr << "Pfad: " << dir << std::endl;
    }
}

void UI::DrawDirectoryContextMenu(const std::filesystem::path& dir, bool isRoot) {
    if (!isRoot && ImGui::BeginPopupContextItem()) {
        if (ImGui::MenuItem("New Folder")) {
            std::string folderName = "New Folder";
            int counter = 1;
            std::filesystem::path newPath = dir / folderName;

            while (std::filesystem::exists(newPath)) {
                newPath = dir / (folderName + " " + std::to_string(counter));
                counter++;
            }

            std::cout << "Kontext-Ordner erstellen:" << std::endl;
            std::cout << "  Parent: " << dir.string() << std::endl;
            std::cout << "  Name: " << newPath.filename().string() << std::endl;

            bool success = ProjectManager::Instance().CreateFolder(dir.string(), newPath.filename().string());
            if (!success) {
                std::cerr << "Fehler beim Erstellen des Kontext-Ordners!" << std::endl;
            }
        }
        if (ImGui::MenuItem("Rename Folder")) {
            pathToRename = dir;
            std::string currentName = dir.filename().string();
            strncpy(renameName, currentName.c_str(), sizeof(renameName) - 1);
            renameName[sizeof(renameName) - 1] = '\0';
            ImGui::OpenPopup("RenameFolderPopup");
        }
        if (ImGui::MenuItem("Delete Folder")) {
            if (std::filesystem::exists(dir)) {
                if (selectedDir == dir) {
                    selectedDir = dir.parent_path();
                }
                std::filesystem::remove_all(dir);
            }
        }
        ImGui::EndPopup();
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
