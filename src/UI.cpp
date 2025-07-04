//
// Created by Anton on 04.07.2025.
//
#include "../include/UI.hpp"

UI::UI(GLFWwindow* window) {
    IMGUI_CHECKVERSION();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

UI::~UI() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void UI::beginFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // DockSpace einrichten
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    ImGui::Begin("DockSpace Demo", nullptr, window_flags);
    ImGui::PopStyleVar(2);

    ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);

    ImGui::End();
}

void UI::endFrame() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

const char* UI::typeToString(LevelObject::Type type) {
    switch (type) {
        case LevelObject::Cube:
            return "Cube";
        case LevelObject::Plane:
            return "Plane";
        default:
            return "Unknown";
    }
}

void UI::draw(const std::vector<Mesh*>& meshes, const Level& level) {
    static int selectedIndex = 0;
    const auto& objects = level.getObjects();

    ImGui::Begin("Scene");
    ImGui::Text("Objects:");
    std::map<LevelObject::Type, int> typeCounter;
    for (int i = 0; i < objects.size(); ++i) {
        int num = ++typeCounter[objects[i].type];
        std::string label = std::string(typeToString(objects[i].type)) + " " + std::to_string(num);
        if (ImGui::Selectable(label.c_str(), selectedIndex == i)) {
            selectedIndex = i;
        }
    }
    ImGui::End();

    ImGui::Begin("Objekt-Daten");
    if (!objects.empty() && selectedIndex >= 0 && selectedIndex < objects.size()) {
        const auto& obj = objects[selectedIndex];
        ImGui::Text("Typ: %s", typeToString(obj.type));
        ImGui::Text("Position: (%.2f, %.2f, %.2f)", obj.position.x, obj.position.y, obj.position.z);
        ImGui::Text("Rotation: Winkel %.2f um Achse (%.2f, %.2f, %.2f)", obj.rotationAngle, obj.rotationAxis.x, obj.rotationAxis.y, obj.rotationAxis.z);
        ImGui::Text("Skalierung: (%.2f, %.2f, %.2f)", obj.scale.x, obj.scale.y, obj.scale.z);
    } else {
        ImGui::Text("Kein Objekt vorhanden.");
    }
    ImGui::End();
}