#pragma once

#include "Renderer/RendererCore.hpp"

#include <glm/glm.hpp>

NAMESPACE_BEGIN(Renderer)

struct Camera
{
    virtual const glm::mat4& GetViewMatrix() const = 0;
    virtual const glm::mat4& GetProjectionMatrix() const = 0;
};

NAMESPACE_END(Renderer)
