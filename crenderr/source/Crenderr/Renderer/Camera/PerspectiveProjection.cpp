#include "PerspectiveProjection.hpp"
#include "Camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

NAMESPACE_BEGIN(Renderer)

glm::mat4 PerspectiveProjection::CalculateViewMatrix(const Camera* const camera) const
{
    const auto radX{ glm::radians(camera->Rotation.x) };
    const auto radY{ glm::radians(camera->Rotation.y) };

    const auto front{ glm::normalize(glm::vec3{
        cos(radX) * cos(radY),
        sin(radY),
        sin(radX) * cos(radY),
    }) };

    const auto right{ glm::normalize(glm::cross(front, c_WorldUp)) };
    const auto up   { glm::normalize(glm::cross(right, front)) };

    return glm::lookAt(camera->Position, camera->Position + front, up);
}

glm::mat4 PerspectiveProjection::CalculateProjectionMatrix() const
{
    return glm::perspective(glm::radians(Fov), Ratio, Near, Far);
}

NAMESPACE_END(Renderer)
