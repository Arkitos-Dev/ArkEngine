#include "../include/Scene.hpp"
#include "../include/Plane.hpp"
#include "../include/Cube.hpp"

Scene::~Scene() {
    for (auto* mesh : meshes)
        delete mesh;
}

void Scene::AddMesh(Mesh* mesh) {
    meshes.push_back(mesh);
}

const std::vector<Mesh*>& Scene::GetMeshes() const {
    return meshes;
}

void Scene::SetLevel(const Level& level) {
    // Vorherige Meshes löschen
    for (auto* mesh : meshes)
        delete mesh;
    meshes.clear();

    for (const auto& obj : level.GetObjects()) {
        Mesh* mesh = nullptr;
        if (obj.type == LevelObject::Cube) {
            mesh = new Cube();
        } else if (obj.type == LevelObject::Plane) {
            mesh = new Plane();
        }
        if (mesh) {
            mesh->SetPosition(obj.position);
            mesh->SetRotation(obj.rotationAngle, obj.rotationAxis);
            mesh->SetScale(obj.scale);
            AddMesh(mesh);
        }
    }
}