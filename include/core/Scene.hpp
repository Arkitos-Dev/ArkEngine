#ifndef INC_3DRENDERER_SCENE_HPP
#define INC_3DRENDERER_SCENE_HPP

#include "../objects/Mesh.hpp"
#include "../objects/Level.hpp"
#include <vector>
#include <string>

#include "Shader.hpp"
#include "glm/glm.hpp"

struct SceneObject {
    enum class ObjectKind { Mesh, Light };
    ObjectKind kind = ObjectKind::Mesh;

    // Gemeinsame Felder
    glm::vec3 position{0,0,0};
    float rotationAngle = 0.0f;
    glm::vec3 rotationAxis{0,1,0};
    glm::vec3 scale{1,1,1};

    // Mesh-spezifisch
    LevelObject::Type type = LevelObject::Cube;
    Mesh* mesh = nullptr;

    // Light-spezifisch
    enum class LightType { Directional, Point, Spot };
    LightType lightType = LightType::Point;
    glm::vec3 color{1,1,1};
    glm::vec3 direction{0,-1,0};
    float constant = 1.0f, linear = 0.09f, quadratic = 0.032f;
    float cutOff = 0.0f, outerCutOff = 0.0f;

    // Konstruktoren für Mesh und Light
    SceneObject(LevelObject::Type t, const glm::vec3& pos, float rot, const glm::vec3& axis, const glm::vec3& sc, Mesh* m)
            : kind(ObjectKind::Mesh), position(pos), rotationAngle(rot), rotationAxis(axis), scale(sc), type(t), mesh(m) {}

    SceneObject(LightType lt, const glm::vec3& pos, const glm::vec3& col, const glm::vec3& dir)
            : kind(ObjectKind::Light), position(pos), color(col), direction(dir), lightType(lt) {}
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