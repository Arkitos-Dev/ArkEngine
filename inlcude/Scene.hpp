//
// Created by Anton on 03.07.2025.
//

#ifndef INC_3DRENDERER_SCENE_HPP
#define INC_3DRENDERER_SCENE_HPP

#include <vector>
#include "Mesh.hpp"

class Scene {
public:
    void addMesh(Mesh* mesh);
    const std::vector<Mesh*>& getMeshes() const;
    ~Scene();
private:
    std::vector<Mesh*> meshes;
};

#endif //INC_3DRENDERER_SCENE_HPP
