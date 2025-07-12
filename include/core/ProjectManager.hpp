//
// Created by Anton on 11.07.2025.
//
#pragma once
#include <string>
#include <map>
#include <memory>
#include <vector>
#include <filesystem>
#include <nlohmann/json.hpp>
#include <cstdlib>
#include <iostream>

struct AssetMeta {
    std::string uuid;
    std::string name;
    std::string path;
    std::string type;
    std::vector<std::string> tags;
    std::string importDate;
};

class ProjectManager {
public:
    static ProjectManager& Instance();

    // Erstellt ein neues Projekt mit Ordnerstruktur und Settings
    void CreateProject(const std::string& rootPath);

    // Lädt ein bestehendes Projekt (Settings und Asset-Metadaten)
    bool LoadProject(const std::string& projectFile);

    // Speichert das aktuelle Projekt (Settings und Asset-Metadaten)
    void SaveProject(const std::string& projectFile) const;

    // Asset-Import (kopiert Datei in Asset-Ordner, legt Metadaten an)
    void ImportAsset(const std::string& filePath, const std::string& type);

    bool CreateFolder(const std::string& parentPath, const std::string& folderName);

    const AssetMeta* GetAssetMeta(const std::string& uuid) const;
    const std::map<std::string, AssetMeta>& GetAllAssets() const;

    // Projekt wechseln
    bool SwitchProject(const std::string& rootPath);

    // Liste aller Projekte
    const std::vector<std::string>& GetAllProjects() const;

    // Projekt-Root abfragen
    const std::string& GetProjectRoot() const { return projectRoot; }

private:
    ProjectManager() = default;
    std::string projectRoot;
    std::vector<std::string> allProjects;
    std::map<std::string, AssetMeta> assets;

    void CreateAssetFolders(const std::string& rootPath);
    void SaveSettings(const std::string& settingsFile) const;
    bool LoadSettings(const std::string& settingsFile);
};