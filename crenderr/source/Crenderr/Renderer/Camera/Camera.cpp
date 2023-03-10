#include "Camera.hpp"

NAMESPACE_BEGIN(Renderer)

Camera::Camera(std::shared_ptr<Projection> projection)
    : ProjectionHandle{ projection } {}

glm::mat4 Camera::CalculateViewMatrix() const
{
    return ProjectionHandle->CalculateViewMatrix(this);
}

glm::mat4 Camera::CalculateProjectionMatrix() const
{
    return ProjectionHandle->CalculateProjectionMatrix();
}

NAMESPACE_END(Renderer)
