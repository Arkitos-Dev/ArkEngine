#include "../../include/objects/Level.hpp"
#include <fstream>

static void writeVec3(std::ofstream& out, const glm::vec3& v) {
    out.write(reinterpret_cast<const char*>(&v.x), sizeof(float));
    out.write(reinterpret_cast<const char*>(&v.y), sizeof(float));
    out.write(reinterpret_cast<const char*>(&v.z), sizeof(float));
}

static void readVec3(std::ifstream& in, glm::vec3& v) {
    in.read(reinterpret_cast<char*>(&v.x), sizeof(float));
    in.read(reinterpret_cast<char*>(&v.y), sizeof(float));
    in.read(reinterpret_cast<char*>(&v.z), sizeof(float));
}

void Level::Save(const std::string& filename) const {
    std::ofstream out(filename, std::ios::binary);
    size_t count = objects.size();
    out.write(reinterpret_cast<const char*>(&count), sizeof(count));
    for (const auto& obj : objects) {
        int type = static_cast<int>(obj.type);
        out.write(reinterpret_cast<const char*>(&type), sizeof(type));
        writeVec3(out, obj.position);
        out.write(reinterpret_cast<const char*>(&obj.rotationAngle), sizeof(obj.rotationAngle));
        writeVec3(out, obj.rotationAxis);
        writeVec3(out, obj.scale);
    }
}

bool Level::Load(const std::string& filename) {
    std::ifstream in(filename, std::ios::binary);
    if (!in) return false;
    size_t count = 0;
    in.read(reinterpret_cast<char*>(&count), sizeof(count));
    if (!in || count > 10000) return false;
    objects.clear();
    for (size_t i = 0; i < count; ++i) {
        LevelObject obj;
        int type = 0;
        in.read(reinterpret_cast<char*>(&type), sizeof(type));
        obj.type = static_cast<LevelObject::Type>(type);
        readVec3(in, obj.position);
        in.read(reinterpret_cast<char*>(&obj.rotationAngle), sizeof(obj.rotationAngle));
        readVec3(in, obj.rotationAxis);
        readVec3(in, obj.scale);
        if (!in) { objects.clear(); return false; }
        objects.push_back(obj);
    }
    return true;
}