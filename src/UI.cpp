//
// Created by Anton on 04.07.2025.
//
#include "../include/UI.hpp"

UI::UI(GLFWwindow* window) {
    IMGUI_CHECKVERSION();
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
}

void UI::endFrame() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UI::draw(const std::vector<Mesh*>& meshes) {
    ImGui::Begin("Szene");
    ImGui::Text("Objekte:");
    for (size_t i = 0; i < meshes.size(); ++i) {
        const auto* mesh = meshes[i];
        std::string label = mesh->name.empty() ? "<unnamed>##" + std::to_string(i) : mesh->name;
        ImGui::Selectable(label.c_str());
    }
    ImGui::End();

    ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
    ImGui::Text("Hier kommt das gerenderte Bild hin.");
    ImGui::End();
}