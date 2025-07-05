#include "../../include/core/Scene.hpp"
#include "../../include/objects/Plane.hpp"
#include "../../include/objects/Cube.hpp"

Scene::~Scene() {
    for (auto& obj : objects)
        delete obj.mesh;
}

void Scene::AddObject(LevelObject::Type type,
                      const glm::vec3& position,
                      float rotationAngle,
                      const glm::vec3& rotationAxis,
                      const glm::vec3& scale) {
    Mesh* mesh = nullptr;
    if (type == LevelObject::Cube) mesh = new Cube();
    else if (type == LevelObject::Plane) mesh = new Plane();
    if (mesh) {
        mesh->SetPosition(position);
        mesh->SetRotation(rotationAngle, rotationAxis);
        mesh->SetScale(scale);
        objects.emplace_back(type, position, rotationAngle, rotationAxis, scale, mesh);
    }
}

void Scene::RemoveObjectAt(size_t index) {
    if (index >= objects.size()) return;
    delete objects[index].mesh;
    objects.erase(objects.begin() + index);
}

void Scene::UpdateScene(size_t index, const SceneObject& obj) {
    if (index >= objects.size()) return;
    if (objects[index].type != obj.type) {
        delete objects[index].mesh;
        if (obj.type == LevelObject::Cube)
            objects[index].mesh = new Cube();
        else if (obj.type == LevelObject::Plane)
            objects[index].mesh = new Plane();
    }
    objects[index].type = obj.type;
    objects[index].position = obj.position;
    objects[index].rotationAngle = obj.rotationAngle;
    objects[index].rotationAxis = obj.rotationAxis;
    objects[index].scale = obj.scale;
    if (objects[index].mesh) {
        objects[index].mesh->SetPosition(obj.position);
        objects[index].mesh->SetRotation(obj.rotationAngle, obj.rotationAxis);
        objects[index].mesh->SetScale(obj.scale);
    }
}

void Scene::Clear() {
    for (auto& obj : objects)
        delete obj.mesh;
    objects.clear();
}

std::vector<SceneObject>& Scene::GetObjects() { return objects; }
const std::vector<SceneObject>& Scene::GetObjects() const { return objects; }

// Neu: Szene aus Level aufbauen
void Scene::FromLevel(const Level& level) {
    Clear();
    for (const auto& obj : level.GetObjects()) {
        AddObject(obj.type, obj.position, obj.rotationAngle, obj.rotationAxis, obj.scale);
    }
}

// Neu: Szene in Level exportieren
void Scene::ToLevel(Level& level) const {
    level.Clear();
    for (const auto& obj : objects) {
        LevelObject lobj;
        lobj.type = obj.type;
        lobj.position = obj.position;
        lobj.rotationAngle = obj.rotationAngle;
        lobj.rotationAxis = obj.rotationAxis;
        lobj.scale = obj.scale;
        level.AddObject(lobj);
    }
}