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

class ArkEngine {
public:
    ArkEngine(int width, int height, const std::string& title);
    ~ArkEngine();
    void Run();
};
