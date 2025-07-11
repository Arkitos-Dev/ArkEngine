//
// Created by Anton on 11.07.2025.
//
#include "../../include/core/ProjectManager.hpp"
#include <fstream>
#include <chrono>
#include <iomanip>
#include <random>

using json = nlohmann::json;
namespace fs = std::filesystem;

ProjectManager& ProjectManager::Instance() {
    static ProjectManager instance;
    return instance;
}

// Hilfsfunktion für den Projektpfad
static std::string GetProjectAssetPath(const std::string& relativePath) {
    std::filesystem::path p(relativePath);
    if (p.is_absolute())
        return relativePath;
    return ProjectManager::Instance().GetProjectRoot() + "/" + relativePath;
}


void ProjectManager::CreateProject(const std::string& projectName) {
    const char* userProfile = std::getenv("USERPROFILE");
    if (!userProfile) return;
    std::string engineRoot = std::string(userProfile) + "\\ArkEngineProjects";
    std::filesystem::create_directories(engineRoot);

    std::string projectRoot = engineRoot + "\\" + projectName;
    this->projectRoot = projectRoot;
    std::filesystem::create_directories(projectRoot + "/assets/models");
    std::filesystem::create_directories(projectRoot + "/assets/textures");
    std::filesystem::create_directories(projectRoot + "/assets/materials");
    std::filesystem::create_directories(projectRoot + "/assets/scenes"); // <-- Hinzugefügt
    std::filesystem::create_directories(projectRoot + "/scenes");
    SaveSettings(projectRoot + "/ProjectSettings.json");
    assets.clear();
    allProjects.push_back(projectRoot);
}

void ProjectManager::CreateAssetFolders(const std::string& rootPath) {
    fs::create_directories(rootPath + "/assets/models");
    fs::create_directories(rootPath + "/assets/textures");
    fs::create_directories(rootPath + "/assets/materials");
}

void ProjectManager::SaveSettings(const std::string& settingsFile) const {
    json j;
    j["projectRoot"] = projectRoot;
    j["assets"] = json::array();
    for (const auto& [uuid, meta] : assets) {
        j["assets"].push_back({
                                      {"uuid", meta.uuid},
                                      {"name", meta.name},
                                      {"path", meta.path},
                                      {"type", meta.type},
                                      {"tags", meta.tags},
                                      {"importDate", meta.importDate}
                              });
    }
    std::ofstream out(settingsFile);
    out << std::setw(4) << j;
}

bool ProjectManager::LoadSettings(const std::string& settingsFile) {
    std::ifstream in(settingsFile);
    if (!in) return false;
    json j;
    in >> j;
    projectRoot = j.value("projectRoot", "");
    assets.clear();
    for (const auto& a : j["assets"]) {
        AssetMeta meta;
        meta.uuid = a.value("uuid", "");
        meta.name = a.value("name", "");
        meta.path = a.value("path", "");
        meta.type = a.value("type", "");
        meta.tags = a.value("tags", std::vector<std::string>{});
        meta.importDate = a.value("importDate", "");
        assets[meta.uuid] = meta;
    }
    return true;
}

bool ProjectManager::LoadProject(const std::string& projectFile) {
    return LoadSettings(projectFile);
}

void ProjectManager::SaveProject(const std::string& projectFile) const {
    SaveSettings(projectFile);
}

static std::string GenerateUUID() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(0, 15);
    std::string uuid = "xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx";
    for (auto& c : uuid) {
        if (c == 'x') c = "0123456789abcdef"[dis(gen)];
        else if (c == 'y') c = "89ab"[dis(gen) % 4];
    }
    return uuid;
}

void ProjectManager::ImportAsset(const std::string& filePath, const std::string& type) {
    std::string assetFolder = projectRoot + "/assets/" + type + "s";
    fs::create_directories(assetFolder);
    std::string filename = fs::path(filePath).filename().string();
    std::string destPath = assetFolder + "/" + filename;
    try {
        if (fs::exists(destPath)) {
            // Optional: Datei vorher löschen
            fs::remove(destPath);
        }
        fs::copy_file(filePath, destPath, fs::copy_options::overwrite_existing);
    } catch (const fs::filesystem_error& e) {
        // Fehler ausgeben, aber nicht crashen
        std::cerr << "ImportAsset Fehler: " << e.what() << std::endl;
        return;
    }

    AssetMeta meta;
    meta.uuid = GenerateUUID();
    meta.name = filename;
    meta.path = "assets/" + type + "s/" + filename;
    meta.type = type;
    meta.importDate = std::to_string(std::time(nullptr));
    assets[meta.uuid] = meta;
}

const AssetMeta* ProjectManager::GetAssetMeta(const std::string& uuid) const {
    auto it = assets.find(uuid);
    if (it != assets.end()) return &it->second;
    return nullptr;
}

const std::map<std::string, AssetMeta>& ProjectManager::GetAllAssets() const {
    return assets;
}

bool ProjectManager::SwitchProject(const std::string& rootPath) {
    std::string settingsFile = rootPath + "/ProjectSettings.json";
    if (!fs::exists(settingsFile)) return false;
    return LoadSettings(settingsFile);
}

const std::vector<std::string>& ProjectManager::GetAllProjects() const {
    return allProjects;
}