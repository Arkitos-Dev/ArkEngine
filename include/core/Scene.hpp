//
// Created by Anton on 03.07.2025.
//

#ifndef INC_3DRENDERER_SCENE_HPP
#define INC_3DRENDERER_SCENE_HPP

#include <vector>
#include "../objects/Mesh.hpp"
#include "../objects/Level.hpp"
#include <string>

class Scene {
public:
    Scene() = default;
    ~Scene();
    void AddMesh(Mesh* mesh);
    const std::vector<Mesh*>& GetMeshes() const;
    void SetLevel(const Level& level);
    void UpdateScene(size_t index, const LevelObject& obj);
    void AddMeshForObject(const LevelObject& obj);
    void RemoveMeshAt(size_t index);
private:
    std::vector<Mesh*> meshes;
};

#endif //INC_3DRENDERER_SCENE_HPP
