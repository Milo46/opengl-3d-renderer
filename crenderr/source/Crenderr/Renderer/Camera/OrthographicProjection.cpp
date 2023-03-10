#include "OrthographicProjection.hpp"
#include "Camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

NAMESPACE_BEGIN(Renderer)

glm::mat4 OrthographicProjection::CalculateViewMatrix(const Camera* const camera) const
{
    auto transform{ glm::identity<glm::mat4>() };
    transform = glm::translate(transform, camera->Position);
    transform = glm::rotate(transform, glm::radians(camera->Rotation.z), { 0.0f, 0.0f, 1.0f, });

    return transform;
}

glm::mat4 OrthographicProjection::CalculateProjectionMatrix() const
{
    return glm::ortho(Left, Right, Bottom, Right);
}

NAMESPACE_END(Renderer)
