//
// Created by Anton on 04.07.2025.
//
#include "../include/Level.hpp"
#include <fstream>

void writeVec3(std::ofstream& out, const glm::vec3& v) {
    out.write(reinterpret_cast<const char*>(&v.x), sizeof(float));
    out.write(reinterpret_cast<const char*>(&v.y), sizeof(float));
    out.write(reinterpret_cast<const char*>(&v.z), sizeof(float));
}

void readVec3(std::ifstream& in, glm::vec3& v) {
    in.read(reinterpret_cast<char*>(&v.x), sizeof(float));
    in.read(reinterpret_cast<char*>(&v.y), sizeof(float));
    in.read(reinterpret_cast<char*>(&v.z), sizeof(float));
}

void Level::SaveLevel(const Level& level, const std::string& filename) {
    std::ofstream out(filename, std::ios::binary);
    size_t count = level.getObjects().size();
    out.write(reinterpret_cast<const char*>(&count), sizeof(count));
    for (const auto& obj : level.getObjects()) {
        int type = static_cast<int>(obj.type);
        out.write(reinterpret_cast<const char*>(&type), sizeof(type));
        writeVec3(out, obj.position);
        out.write(reinterpret_cast<const char*>(&obj.rotationAngle), sizeof(obj.rotationAngle));
        writeVec3(out, obj.rotationAxis);
        writeVec3(out, obj.scale);
    }
}

void Level::LoadLevel(Level& level, const std::string& filename) {
    std::ifstream in(filename, std::ios::binary);
    if (!in) return;

    size_t count = 0;
    in.read(reinterpret_cast<char*>(&count), sizeof(count));
    if (!in || count > 10000) return;

    level.clear();
    level.getObjects().resize(count);
    for (size_t i = 0; i < count; ++i) {
        auto& obj = level.getObjects()[i];
        int type = 0;
        in.read(reinterpret_cast<char*>(&type), sizeof(type));
        obj.type = static_cast<LevelObject::Type>(type);
        readVec3(in, obj.position);
        in.read(reinterpret_cast<char*>(&obj.rotationAngle), sizeof(obj.rotationAngle));
        readVec3(in, obj.rotationAxis);
        readVec3(in, obj.scale);
        if (!in) { level.clear(); return; }
    }
}