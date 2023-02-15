#include "OrthographicCamera.hpp"

#include <glm/gtc/matrix_transform.hpp>

NAMESPACE_BEGIN(Renderer)

glm::mat4 OrthographicProjection::Calculate(const OrthographicProjection& projection)
{
    return glm::ortho(projection.Left, projection.Right, projection.Bottom, projection.Top);
}

OrthographicCamera::OrthographicCamera(const OrthographicProjection& projection)
    : m_ProjectionMatrix{ OrthographicProjection::Calculate(projection) },
      m_ViewMatrix{ glm::identity<glm::mat4>() }
{
    OrthographicCamera::RecalculateViewMatrix();
}

OrthographicCamera& OrthographicCamera::SetPosition(const glm::vec3& position) noexcept
{
    m_Position = position;
    OrthographicCamera::RecalculateViewMatrix();

    return *this;
}

OrthographicCamera& OrthographicCamera::SetRotation(const float rotation) noexcept
{
    m_Rotation = rotation;
    OrthographicCamera::RecalculateViewMatrix();

    return *this;
}

const glm::mat4& OrthographicCamera::GetViewMatrix() const
{
    return m_ViewMatrix;
}

const glm::mat4& OrthographicCamera::GetProjectionMatrix() const
{
    return m_ProjectionMatrix;
}

void OrthographicCamera::RecalculateViewMatrix()
{
    auto transform{ glm::identity<glm::mat4>() };
    transform = glm::translate(transform, m_Position);
    transform = glm::rotate(transform, glm::radians(m_Rotation), { 0.0f, 0.0f, 1.0f, });

    m_ViewMatrix = glm::inverse(transform);
}

NAMESPACE_END(Renderer)
