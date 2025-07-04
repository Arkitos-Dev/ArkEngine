//
// Created by Anton on 03.07.2025.
//

#ifndef INC_3DRENDERER_SCENE_HPP
#define INC_3DRENDERER_SCENE_HPP

#include <vector>
#include "Mesh.hpp"
#include "Level.hpp"
#include <string>

class Scene {
public:
    Scene() = default;
    ~Scene();
    void addMesh(Mesh* mesh);
    const std::vector<Mesh*>& getMeshes() const;
    void loadLevel(const Level& level);
private:
    std::vector<Mesh*> meshes;
};

#endif //INC_3DRENDERER_SCENE_HPP
