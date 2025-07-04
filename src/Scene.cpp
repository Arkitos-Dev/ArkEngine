//
// Created by Anton on 03.07.2025.
//

#include "../include/Scene.hpp"

Scene::~Scene() = default;

void Scene::addMesh(Mesh* mesh) {
    meshes.push_back(mesh);
}

const std::vector<Mesh*>& Scene::getMeshes() const {
    return meshes;
}