#include "../../include/core/Scene.hpp"
#include <fstream>

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