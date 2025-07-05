//
// Created by Anton on 04.07.2025.
//
#include "../include/UI.hpp"

UI::UI(GLFWwindow* window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); // Optional, falls du io brauchst
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

UI::~UI() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void UI::BeginFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar; // <- Kein NoDocking mehr!
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    ImGui::Begin("DockSpace", nullptr, window_flags);
    ImGui::PopStyleVar(2);

    ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

    ImGui::End();
}

void UI::EndFrame() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}

const char* UI::TypeToString(LevelObject::Type type) {
    switch (type) {
        case LevelObject::Cube:
            return "Cube";
        case LevelObject::Plane:
            return "Plane";
        default:
            return "Unknown";
    }
}

// C++
void UI::Draw(const std::vector<Mesh*>& meshes, Level& level, Scene& scene) {
    static int selectedIndex = 0;
    auto& objects = level.getObjects();

    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            // Im "File"-Menü im UI::draw
            if (ImGui::MenuItem("Save")) {
                level.SaveLevel(level, "level.bin");
            }
            if (ImGui::MenuItem("Load")) {
                level.LoadLevel(level, "level.bin");
                scene.LoadLevel(level); // Szene nach dem Laden aktualisieren
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Bearbeiten")) {
            if (ImGui::MenuItem("Rückgängig", "Strg+Z")) {
                // TODO: Undo
            }
            if (ImGui::MenuItem("Wiederholen", "Strg+Y")) {
                // TODO: Redo
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    ImGui::Begin("Scene");
    ImGui::Text("Objects:");
    std::map<LevelObject::Type, int> typeCounter;
    bool anyItemHovered = false;
    bool objectMenuOpened = false;
    for (int i = 0; i < objects.size(); ++i) {
        int num = ++typeCounter[objects[i].type];
        std::string label = std::string(TypeToString(objects[i].type)) + " " + std::to_string(num);
        bool selected = (selectedIndex == i);
        ImGui::Selectable(label.c_str(), selected);

        if (ImGui::IsItemHovered())
            anyItemHovered = true;

        if (ImGui::IsItemClicked())
            selectedIndex = i;

        // Eindeutiger Popup-Name pro Objekt
        std::string popupId = "ObjectContextMenu_" + std::to_string(i);
        if (selected && ImGui::IsItemClicked(ImGuiMouseButton_Right))
            ImGui::OpenPopup(popupId.c_str());

        if (ImGui::BeginPopup(popupId.c_str())) {
            objectMenuOpened = true;
            if (ImGui::MenuItem("Delete Object")) {
                objects.erase(objects.begin() + i);
                if (selectedIndex >= objects.size())
                    selectedIndex = static_cast<int>(objects.size()) - 1;
                scene.LoadLevel(level);
                ImGui::CloseCurrentPopup();
                break;
            }
            ImGui::EndPopup();
        }
    }

// Fenster-Kontextmenü nur, wenn kein Objekt-Menü offen und kein Item hovered
    if (!anyItemHovered && !objectMenuOpened &&
        ImGui::BeginPopupContextWindow("SceneContextMenu", ImGuiPopupFlags_MouseButtonRight)) {
        if (ImGui::BeginMenu("Add Object")) {
            if (ImGui::MenuItem("Cube")) {
                LevelObject obj;
                obj.type = LevelObject::Cube;
                obj.position = {0, 0, 0};
                obj.rotationAngle = 0.0f;
                obj.rotationAxis = {0, 1, 0};
                obj.scale = {1, 1, 1};
                level.AddObject(obj);
                selectedIndex = static_cast<int>(objects.size()) - 1;
                scene.LoadLevel(level);
            }
            if (ImGui::MenuItem("Plane")) {
                LevelObject obj;
                obj.type = LevelObject::Plane;
                obj.position = {0, 0, 0};
                obj.rotationAngle = 0.0f;
                obj.rotationAxis = {0, 1, 0};
                obj.scale = {1, 1, 1};
                level.AddObject(obj);
                selectedIndex = static_cast<int>(objects.size()) - 1;
                scene.LoadLevel(level);
            }
            ImGui::EndMenu();
        }
        ImGui::EndPopup();
    }
    ImGui::End();

    ImGui::Begin("Objekt-Daten");
    if (!objects.empty() && selectedIndex >= 0 && selectedIndex < objects.size()) {
        auto& obj = objects[selectedIndex];
        ImGui::Text("Typ: %s", TypeToString(obj.type));
        bool changed = false;
        changed |= ImGui::DragFloat3("Position", &obj.position.x, 0.05f);
        changed |= ImGui::DragFloat("Rotation", &obj.rotationAngle, 1.0f, -360.0f, 360.0f);
        changed |= ImGui::DragFloat3("Rotationsachse", &obj.rotationAxis.x, 0.05f);
        changed |= ImGui::DragFloat3("Skalierung", &obj.scale.x, 0.05f, 0.01f, 100.0f);
        if (changed) {
            scene.LoadLevel(level);
        }
    } else {
        ImGui::Text("Kein Objekt vorhanden.");
    }
    ImGui::End();
}

ImVec2 UI::DrawViewport(GLuint texture, int texWidth, int texHeight) {
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

    ImGui::Image((void*)(intptr_t)texture, imageSize, ImVec2(0,1), ImVec2(1,0));
    ImGui::End();
    return imageSize;
}