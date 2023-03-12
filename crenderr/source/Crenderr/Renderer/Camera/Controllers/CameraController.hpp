#pragma once

#include "Renderer/RendererCore.hpp"
#include "Renderer/Camera/Camera.hpp"

NAMESPACE_BEGIN(Renderer)

struct CameraController {};

struct CameraOrbitController : public CameraController
{
    Camera CameraHandle{};

    glm::vec2 FocusPoint{ glm::vec2(0.0f) };
    float ArmLength{ 1.0f };

    CameraOrbitController() = default;
    
};

NAMESPACE_END(Renderer)
