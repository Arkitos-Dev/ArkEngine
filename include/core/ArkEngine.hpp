//
// Created by Anton on 05.07.2025.
//
#pragma once
#include "Window.hpp"
#include "Camera.hpp"
#include "Shader.hpp"
#include "Renderer.hpp"
#include "Scene.hpp"
#include "ResourceManager.hpp"
#include "UI.hpp"
#include "ProjectManager.hpp"

class ArkEngine {
public:
    ArkEngine();
    ~ArkEngine();
    void Run();
};
