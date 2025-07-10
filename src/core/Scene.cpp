#include "../../include/core/Scene.hpp"
#include "../../include/objects/Plane.hpp"
#include "../../include/objects/Cube.hpp"
#include "../../include/objects/Level.hpp"

Scene::~Scene() {
    Clear();
}

void Scene::AddObject(std::unique_ptr<GameObject> obj) {
    objects.push_back(std::move(obj));
}

void Scene::RemoveObjectAt(size_t index) {
    if (index < objects.size())
        objects.erase(objects.begin() + index);
}

void Scene::Clear() {
    objects.clear();
}

std::vector<std::unique_ptr<GameObject>>& Scene::GetObjects() {
    return objects;
}

const std::vector<std::unique_ptr<GameObject>>& Scene::GetObjects() const {
    return objects;
}