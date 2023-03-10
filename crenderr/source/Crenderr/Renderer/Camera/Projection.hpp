#pragma once

#include "Renderer/RendererCore.hpp"

#include <glm/glm.hpp>

NAMESPACE_BEGIN(Renderer)

struct Camera;

struct Projection
{
    virtual glm::mat4 CalculateViewMatrix(const Camera* const camera) const = 0;
    virtual glm::mat4 CalculateProjectionMatrix() const = 0;
};

NAMESPACE_END(Renderer)
