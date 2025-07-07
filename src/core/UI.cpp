// C++
#include "../../include/core/UI.hpp"
#include "../../include/objects/Level.hpp"

UI::UI(GLFWwindow* window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
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
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

    ImGui::End();
}

void UI::EndFrame() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

const char* UI::TypeToString(LevelObject::Type type) {
    switch (type) {
        case LevelObject::Cube:  return "Cube";
        case LevelObject::Plane: return "Plane";
        default:                 return "Unknown";
    }
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
                Level level;
                scene.ToLevel(level);
                level.Save("level.bin");
            }
            if (ImGui::MenuItem("Load")) {
                Level level;
                if (level.Load("level.bin")) {
                    scene.FromLevel(level);
                }
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
    std::map<LevelObject::Type, int> typeCounter;
    bool anyItemHovered = false;

    for (int i = 0; i < objects.size(); ++i) {
        LevelObject::Type type = objects[i].type;
        int num = ++typeCounter[type];
        std::string label = std::string(TypeToString(type)) + " " + std::to_string(num);
        bool selected = (selectedIndex == i);
        if (ImGui::Selectable(label.c_str(), selected)) {
            selectedIndex = i;
        }
        if (ImGui::IsItemHovered())
            anyItemHovered = true;

        if (ImGui::BeginPopupContextItem()) {
            if (ImGui::MenuItem("Delete Object")) {
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

    if (!anyItemHovered && ImGui::BeginPopupContextWindow("SceneContextMenu", ImGuiPopupFlags_MouseButtonRight)) {
        if (ImGui::BeginMenu("Add Object")) {
            if (ImGui::MenuItem("Cube")) {
                scene.AddObject(LevelObject::Cube);
                selectedIndex = static_cast<int>(scene.GetObjects().size()) - 1;
            }
            if (ImGui::MenuItem("Plane")) {
                scene.AddObject(LevelObject::Plane);
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
        ImGui::Text("Typ: %s", TypeToString(obj.type));
        bool changed = false;
        changed |= ImGui::DragFloat3("Position", &obj.position.x, 0.05f);
        changed |= ImGui::DragFloat("Rotation", &obj.rotationAngle, 1.0f, -360.0f, 360.0f);
        changed |= ImGui::DragFloat3("Rotationsachse", &obj.rotationAxis.x, 0.05f);
        changed |= ImGui::DragFloat3("Skalierung", &obj.scale.x, 0.05f, 0.01f, 100.0f);
        if (changed) {
            scene.UpdateScene(selectedIndex, obj);
        }

        static const char* textureOptions[] = { "resources/images/awesomeface.png", "resources/images/container.jpg", "resources/images/container2.png", "resources/images/container2_specular.png" };
        static int currentTexture = 0;
        auto* mesh = obj.mesh;

        const char* currentTexturePath = nullptr;
        for (int i = 0; i < IM_ARRAYSIZE(textureOptions); ++i) {
            if (ResourceManager::GetTexture(textureOptions[i]) == mesh->GetTextureID()) {
                currentTexture = i;
                currentTexturePath = textureOptions[i];
                break;
            }
        }
        if (!currentTexturePath) currentTexturePath = "Unbekannt";

        ImGui::Text("Aktuelle Textur: %s", currentTexturePath);
        if (ImGui::Combo("Textur wählen", &currentTexture, textureOptions, IM_ARRAYSIZE(textureOptions))) {
            mesh->SetTexture(textureOptions[currentTexture]);
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