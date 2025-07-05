#ifndef INC_3DRENDERER_SCENE_HPP
#define INC_3DRENDERER_SCENE_HPP

#include "../objects/Mesh.hpp"
#include "../objects/Level.hpp"
#include <vector>
#include <string>
#include "glm/glm.hpp"

struct SceneObject {
    LevelObject::Type type;
    glm::vec3 position;
    float rotationAngle = 0.0f;
    glm::vec3 rotationAxis = {0,1,0};
    glm::vec3 scale = {1,1,1};
    Mesh* mesh = nullptr;
};

class Scene {
public:
    Scene() = default;
    ~Scene();
    void AddObject(LevelObject::Type type,
                   const glm::vec3& position = {0,0,0},
                   float rotationAngle = 0.0f,
                   const glm::vec3& rotationAxis = {0,1,0},
                   const glm::vec3& scale = {1,1,1});
    void RemoveObjectAt(size_t index);
    void UpdateScene(size_t index, const SceneObject& obj);
    void Clear();
    std::vector<SceneObject>& GetObjects();
    const std::vector<SceneObject>& GetObjects() const;

    // Neu: Import/Export
    void FromLevel(const Level& level);
    void ToLevel(Level& level) const;

private:
    std::vector<SceneObject> objects;
};

#endif //INC_3DRENDERER_SCENE_HPP