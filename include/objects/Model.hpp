//
// Created by Anton on 09.07.2025.
//
#pragma once

#include <vector>
#include <string>
#include "MeshNew.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include "../core/ResourceManager.hpp"

class Model
{
public:
    Model(const std::string& path)
    {
        loadModel(path);
    }
    void Draw(Shader &shader);
private:
    // model data
    std::vector<MeshNew> meshes;
    std::string directory;

    void loadModel(std::string path);
    void processNode(aiNode *node, const aiScene *scene);
    MeshNew processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
                                         std::string typeName);
};
