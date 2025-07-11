#include "../../include/core/UI.hpp"
#include "../../include/objects/Cube.hpp"
#include "../../include/objects/Plane.hpp"
#include "../../include/objects/Light.hpp"
#include "glm/gtx/quaternion.hpp"
#include "ImGuiFileDialog.h"

UI::UI(GLFWwindow* window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
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

    style.WindowRounding = 4.0f;
    style.FrameRounding = 2.0f;
    style.GrabRounding = 2.0f;
    style.ScrollbarRounding = 4.0f;
    style.FrameBorderSize = 1.0f;
    style.WindowBorderSize = 1.0f;

    ImVec4 bg = ImVec4(0.08, 0.08, 0.08, 1.0); // Sehr dunkler Hintergrund
    ImVec4 panel = ImVec4(0.08, 0.08, 0.08, 1.0f); // Panels
    ImVec4 panelHover = ImVec4(0.12f, 0.12f, 0.12f, 1.0f);
    ImVec4 panelActive = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
    ImVec4 text = ImVec4(0.92f, 0.93f, 0.95f, 1.0f);
    ImVec4 textDisabled = ImVec4(0.45f, 0.46f, 0.48f, 1.0f);
    ImVec4 border = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);

    style.Colors[ImGuiCol_SliderGrab]        = ImVec4(0.30f, 0.31f, 0.34f, 1.0f);
    style.Colors[ImGuiCol_SliderGrabActive]  = ImVec4(0.40f, 0.41f, 0.44f, 1.0f);

    style.Colors[ImGuiCol_WindowBg]          = bg;
    style.Colors[ImGuiCol_ChildBg]           = panel;
    style.Colors[ImGuiCol_PopupBg]           = panel;
    style.Colors[ImGuiCol_Border]            = border;
    style.Colors[ImGuiCol_BorderShadow]      = ImVec4(0,0,0,0);

    style.Colors[ImGuiCol_FrameBg]        = ImVec4(0.2f, 0.2f, 0.2f, 1.0f); // Grundfarbe der Felder
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.22f, 0.22f, 0.22f, 1.0f); // Hover
    style.Colors[ImGuiCol_FrameBgActive]  = ImVec4(0.25f, 0.25f, 0.25f, 1.0f); // Aktiv

    style.Colors[ImGuiCol_TitleBg]           = panel;
    style.Colors[ImGuiCol_TitleBgActive]     = panelActive;
    style.Colors[ImGuiCol_TitleBgCollapsed]  = bg;

    style.Colors[ImGuiCol_MenuBarBg]         = panel;

    style.Colors[ImGuiCol_ScrollbarBg]       = panel;
    style.Colors[ImGuiCol_ScrollbarGrab]     = ImVec4(0.20f, 0.21f, 0.24f, 1.0f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = panelHover;
    style.Colors[ImGuiCol_ScrollbarGrabActive]  = panelActive;

    style.Colors[ImGuiCol_CheckMark]         = ImVec4(0.70f, 0.71f, 0.73f, 1.0f);

    style.Colors[ImGuiCol_SliderGrab]        = ImVec4(0.30f, 0.31f, 0.34f, 1.0f);
    style.Colors[ImGuiCol_SliderGrabActive]  = ImVec4(0.40f, 0.41f, 0.44f, 1.0f);

    style.Colors[ImGuiCol_Button]            = ImVec4(0.20f, 0.21f, 0.24f, 1.0f);
    style.Colors[ImGuiCol_ButtonHovered]     = ImVec4(0.22f, 0.23f, 0.26f, 1.0f);
    style.Colors[ImGuiCol_ButtonActive]      = ImVec4(0.24f, 0.26f, 0.28f, 1.0f);

    style.Colors[ImGuiCol_Header]            = panelHover;
    style.Colors[ImGuiCol_HeaderHovered]     = panelActive;
    style.Colors[ImGuiCol_HeaderActive]      = panelActive;

    style.Colors[ImGuiCol_Separator]         = border;
    style.Colors[ImGuiCol_SeparatorHovered]  = panelHover;
    style.Colors[ImGuiCol_SeparatorActive]   = panelActive;

    style.Colors[ImGuiCol_ResizeGrip]        = panel;
    style.Colors[ImGuiCol_ResizeGripHovered] = panelHover;
    style.Colors[ImGuiCol_ResizeGripActive]  = panelActive;

    style.Colors[ImGuiCol_Tab]                = panel;
    style.Colors[ImGuiCol_TabHovered]         = panelHover;
    style.Colors[ImGuiCol_TabActive]          = panelActive;
    style.Colors[ImGuiCol_TabUnfocused]       = bg;
    style.Colors[ImGuiCol_TabUnfocusedActive] = panel;

    style.Colors[ImGuiCol_Text]              = text;
    style.Colors[ImGuiCol_TextDisabled]      = textDisabled;
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
    DrawFileBrowser();
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
    if (!objects.empty() && selectedIndex >= 0 && selectedIndex < objects.size()) {
        auto& obj = objects[selectedIndex];

        // Typ-Anzeige wie gehabt...

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

std::string UI::selectedFile = "";
bool UI::showFileDialog = false;

void UI::DrawFileBrowser() {
    ImGui::Begin("File Browser");

    if (ImGui::Button("Model wählen")) {
        IGFD::FileDialogConfig modelConfig;
        modelConfig.path = "resources/model";
        ImGuiFileDialog::Instance()->OpenDialog("ChooseModel", "Model auswählen", ".obj,.fbx,.gltf", modelConfig);

    }
    if (ImGui::Button("Textur wählen")) {
        IGFD::FileDialogConfig textureConfig;
        textureConfig.path = "resources/textures";
        ImGuiFileDialog::Instance()->OpenDialog("ChooseTexture", "Textur auswählen", ".png,.jpg,.jpeg", textureConfig);
    }

    // Model-Dialog
    if (ImGuiFileDialog::Instance()->Display("ChooseModel")) {
        if (ImGuiFileDialog::Instance()->IsOk()) {
            selectedFile = ImGuiFileDialog::Instance()->GetFilePathName();
            // Hier Model laden, z.B. ResourceManager::GetModel(selectedFile);
        }
        ImGuiFileDialog::Instance()->Close();
    }
    // Textur-Dialog
    if (ImGuiFileDialog::Instance()->Display("ChooseTexture")) {
        if (ImGuiFileDialog::Instance()->IsOk()) {
            selectedFile = ImGuiFileDialog::Instance()->GetFilePathName();
            // Hier Textur laden, z.B. ResourceManager::GetTexture(selectedFile);
        }
        ImGuiFileDialog::Instance()->Close();
    }

    if (!selectedFile.empty()) {
        ImGui::Text("Ausgewählt: %s", selectedFile.c_str());
    }

    ImGui::End();
}