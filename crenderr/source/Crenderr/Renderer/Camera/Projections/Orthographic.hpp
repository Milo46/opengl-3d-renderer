#pragma once

#include "Renderer/Camera/Projection.hpp"

NAMESPACE_BEGIN(Renderer)

struct OrthographicProjection : public Projection
{
    float Left  { -1.0f };
    float Right {  1.0f };
    float Bottom{ -1.0f };
    float Top   {  1.0f };

    virtual glm::mat4 CalculateViewMatrix(const Camera* const camera) const override;
    virtual glm::mat4 CalculateProjectionMatrix() const override;
};

NAMESPACE_END(Renderer)
