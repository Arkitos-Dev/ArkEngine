//
// Created by Anton on 10.07.2025.
//
#pragma once
#include "GameObject.hpp"
#include "Mesh.hpp"
#include <memory>

class Shapes : public GameObject {
public:
    std::shared_ptr<Mesh> mesh;

    Shapes() = default;
    virtual ~Shapes() = default;

    std::shared_ptr<Mesh> GetMesh() const { return mesh; }
};