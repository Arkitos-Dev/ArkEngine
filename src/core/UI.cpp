#include "../../include/core/UI.hpp"
#include "../../include/objects/Cube.hpp"
#include "../../include/objects/Plane.hpp"
#include "../../include/objects/Light.hpp"
#include "glm/gtx/quaternion.hpp"

UI::UI(GLFWwindow* window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
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

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar;
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
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), 0);

    ImGui::End();
}

void UI::EndFrame() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UI::Draw(const std::vector<Mesh*>& meshes, Scene& scene) {
    static int selectedIndex = 0;
    DrawMainMenu(scene);
    DrawSceneList(scene, selectedIndex);
    DrawObjectInfo(scene, selectedIndex, meshes);
}

void UI::DrawMainMenu(Scene& scene) {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Save")) {
                // Implementierung für Speichern
            }
            if (ImGui::MenuItem("Load")) {
                // Implementierung für Laden
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Bearbeiten")) {
            ImGui::MenuItem("Rückgängig", "Strg+Z");
            ImGui::MenuItem("Wiederholen", "Strg+Y");
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
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
            if (ImGui::MenuItem("Light")) {
                scene.AddObject(std::make_unique<PointLight>());
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
    if (!objects.empty() && selectedIndex >= 0 && selectedIndex < objects.size()) {
        auto& obj = objects[selectedIndex];

        // Typ-Anzeige wie gehabt...

        glm::vec3 pos = obj->GetPosition();
        if (ImGui::DragFloat3("Position", &pos.x, 0.05f))
            obj->SetPosition(pos);

        // Quaternion in Winkel + Achse umwandeln
        glm::quat rot = obj->GetRotation();
        float angleDeg = glm::degrees(glm::angle(rot));
        glm::vec3 axis = glm::axis(rot);

        // UI für Rotation
        bool changed = false;
        changed |= ImGui::DragFloat("Rotation (Grad)", &angleDeg, 1.0f, -360.0f, 360.0f);
        changed |= ImGui::DragFloat3("Rotationsachse", &axis.x, 0.05f);

        if (changed) {
            if (glm::length(axis) < 1e-4f) axis = glm::vec3(0,1,0); // Fallback
            obj->SetRotation(angleDeg, axis);
        }

        glm::vec3 scale = obj->GetScale();
        if (ImGui::DragFloat3("Skalierung", &scale.x, 0.05f, 0.01f, 100.0f))
            obj->SetScale(scale);

        // Light-spezifisch etc. wie gehabt...
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