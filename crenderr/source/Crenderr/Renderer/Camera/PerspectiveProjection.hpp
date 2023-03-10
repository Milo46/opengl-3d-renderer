#pragma once

#include "Projection.hpp"

NAMESPACE_BEGIN(Renderer)

struct PerspectiveProjection : public Projection
{
    const glm::vec3 c_WorldUp{ 0.0f, 1.0f, 0.0f, };

    float Fov{ 45.0f };
    float Ratio{ 1.0f };
    float Near{ 0.1f };
    float Far{ 1000.0f };

    virtual glm::mat4 CalculateViewMatrix(const Camera* const camera) const override;
    virtual glm::mat4 CalculateProjectionMatrix() const override;
};

NAMESPACE_END(Renderer)
