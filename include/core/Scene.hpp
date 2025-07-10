// Scene.hpp
#pragma once
#include <vector>
#include <memory>
#include "../objects/GameObject.hpp"

class Scene {
public:
    Scene() = default;
    ~Scene();

    void AddObject(std::unique_ptr<GameObject> obj);
    void RemoveObjectAt(size_t index);
    void Clear();

    std::vector<std::unique_ptr<GameObject>>& GetObjects();
    const std::vector<std::unique_ptr<GameObject>>& GetObjects() const;

private:
    std::vector<std::unique_ptr<GameObject>> objects;
};