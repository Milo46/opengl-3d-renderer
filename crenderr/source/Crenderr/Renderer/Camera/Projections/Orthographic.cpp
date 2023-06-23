#include "Orthographic.hpp"
#include "Renderer/Camera/Camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

NAMESPACE_BEGIN(Renderer)

glm::mat4 OrthographicProjection::CalculateViewMatrix(const Camera* const camera) const
{
    // auto transform{ glm::identity<glm::mat4>() };
    // transform = glm::translate(transform, camera->Position);
    // transform = glm::rotate(transform, glm::radians(camera->Rotation.z), { 0.0f, 0.0f, 1.0f, });

    // return transform;

    // auto transform{ glm::identity<glm::mat4>() };
    
    // transform = glm::translate(transform, camera->Position);

    // transform = glm::rotate(transform, glm::radians(camera->Rotation.x), { 1.0f, 0.0f, 0.0f, });
    // transform = glm::rotate(transform, glm::radians(camera->Rotation.y), { 0.0f, 1.0f, 0.0f, });
    // transform = glm::rotate(transform, glm::radians(camera->Rotation.z), { 0.0f, 0.0f, 1.0f, });

    // return transform;

    const auto radX{ glm::radians(camera->Rotation.x) };
    const auto radY{ glm::radians(camera->Rotation.y) };

    const auto front{ glm::normalize(glm::vec3{
        cos(radX) * cos(radY),
        sin(radY),
        sin(radX) * cos(radY),
    }) };

    const auto right{ glm::normalize(glm::cross(front, { 0.0f, 1.0f, 0.0f, })) };
    const auto up   { glm::normalize(glm::cross(right, front)) };

    return glm::lookAt(camera->Position, camera->Position + front, up);
}

glm::mat4 OrthographicProjection::CalculateProjectionMatrix() const
{
    return glm::ortho(Left / Zoom, Right / Zoom, Bottom / Zoom, Right / Zoom, -100.0f, 100.0f);
    // return glm::ortho(-1.f, 1.f, -1.f, 1.f, -100.0f, 100.0f);
}

NAMESPACE_END(Renderer)
