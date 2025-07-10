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
    void Save(const std::string& filename) const;
    bool Load(const std::string& filename);

    std::vector<std::unique_ptr<GameObject>>& GetObjects();
    const std::vector<std::unique_ptr<GameObject>>& GetObjects() const;

private:
    std::vector<std::unique_ptr<GameObject>> objects;
};