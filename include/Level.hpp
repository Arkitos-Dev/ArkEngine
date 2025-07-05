// Level.hpp
#ifndef INC_3DRENDERER_LEVEL_HPP
#define INC_3DRENDERER_LEVEL_HPP

#include <vector>
#include <glm/glm.hpp>
#include <string>

struct LevelObject {
    enum Type { Cube, Plane } type;
    glm::vec3 position;
    float rotationAngle = 0.0f;
    glm::vec3 rotationAxis = {0,1,0};
    glm::vec3 scale = {1,1,1};
};

class Level {
public:
    void AddObject(const LevelObject& obj) { objects.push_back(obj); }
    void Clear() { objects.clear(); }
    const std::vector<LevelObject>& GetObjects() const { return objects; }
    std::vector<LevelObject>& GetObjects() { return objects; }
    void SaveLevel(const Level& level, const std::string& filename);
    void LoadLevel(Level& level, const std::string& filename);
private:
    std::vector<LevelObject> objects;
};

#endif //INC_3DRENDERER_LEVEL_HPP