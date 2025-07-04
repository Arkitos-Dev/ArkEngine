// Level.hpp
#ifndef INC_3DRENDERER_LEVEL_HPP
#define INC_3DRENDERER_LEVEL_HPP

#include <vector>
#include <glm/glm.hpp>

struct LevelObject {
    enum Type { Cube, Plane } type;
    glm::vec3 position;
    float rotationAngle = 0.0f;
    glm::vec3 rotationAxis = {0,1,0};
    glm::vec3 scale = {1,1,1};
};

class Level {
public:
    void addObject(const LevelObject& obj) { objects.push_back(obj); }
    void clear() { objects.clear(); }
    const std::vector<LevelObject>& getObjects() const { return objects; }
private:
    std::vector<LevelObject> objects;
};

#endif //INC_3DRENDERER_LEVEL_HPP