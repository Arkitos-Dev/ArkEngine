#ifndef INC_3DRENDERER_SCENE_HPP
#define INC_3DRENDERER_SCENE_HPP

#include "../objects/Mesh.hpp"
#include "../objects/Level.hpp"
#include <vector>
#include <string>

#include "Shader.hpp"
#include "glm/glm.hpp"

// SceneObject mit Light-Unterstützung
struct SceneObject {
    enum class ObjectKind { Mesh, Light };
    ObjectKind kind = ObjectKind::Mesh;
    LevelObject::Type type;
    glm::vec3 position;
    float rotationAngle = 0.0f;
    glm::vec3 rotationAxis = {0,1,0};
    glm::vec3 scale = {1,1,1};
    Mesh* mesh = nullptr;
    Shader* shader = nullptr;

    // Für Lichtobjekte:
    enum class LightType { Directional, Point, Spot };
    LightType lightType;
    glm::vec3 color = {1,1,1};
    glm::vec3 direction = {0,-1,0};
    // Attenuation für Point/Spot
    float constant = 1.0f, linear = 0.09f, quadratic = 0.032f;
    // Spot-spezifisch
    float cutOff = 0.0f, outerCutOff = 0.0f;

    // In Scene.hpp, innerhalb von struct SceneObject:
    SceneObject(LevelObject::Type type,
                const glm::vec3& position,
                float rotationAngle,
                const glm::vec3& rotationAxis,
                const glm::vec3& scale,
                Mesh* mesh)
            : kind(ObjectKind::Mesh),
              type(type),
              position(position),
              rotationAngle(rotationAngle),
              rotationAxis(rotationAxis),
              scale(scale),
              mesh(mesh),
              shader(nullptr),
              lightType(LightType::Point),
              color(1,1,1),
              direction(0,-1,0),
              constant(1.0f), linear(0.09f), quadratic(0.032f),
              cutOff(0.0f), outerCutOff(0.0f)
    {}
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