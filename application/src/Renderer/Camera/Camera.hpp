#pragma once

#include "Renderer/RendererCore.hpp"

#include <glm/glm.hpp>

RENDERER_CODE_BEGIN

struct Camera
{
    Camera() = default;
    virtual ~Camera() = default;

    virtual const glm::mat4& GetViewMatrix() const = 0;
    virtual const glm::mat4& GetProjectionMatrix() const = 0;
};

RENDERER_CODE_END
