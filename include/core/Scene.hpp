#pragma once
#include <vector>
#include <memory>
#include "../objects/GameObject.hpp"

class Scene {
public:
    Scene() = default;
    ~Scene();

    void AddObject(std::shared_ptr<GameObject> obj);
    void RemoveObjectAt(size_t index);
    void Clear();

    std::vector<std::shared_ptr<GameObject>>& GetObjects();
    const std::vector<std::shared_ptr<GameObject>>& GetObjects() const;

    void Save(const std::string& filename) const;
    bool Load(const std::string& filename);

private:
    std::vector<std::shared_ptr<GameObject>> objects;
};