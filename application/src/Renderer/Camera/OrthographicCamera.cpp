#include "OrthographicCamera.hpp"

#include <glm/gtc/matrix_transform.hpp>

RENDERER_CODE_BEGIN

OrthographicCamera::OrthographicCamera(const OrthographicProjection& projection)
    : m_ProjectionMatrix{ glm::ortho(projection.Left, projection.Right, projection.Bottom, projection.Top, 0.0f, 100.0f) },
      m_ViewMatrix{ glm::identity<glm::mat4>() }
{
    OrthographicCamera::UpdateData();
}

void OrthographicCamera::SetProjection(const OrthographicProjection& projection)
{
    m_ProjectionMatrix = glm::ortho(projection.Left, projection.Right, projection.Bottom, projection.Top, 0.0f, 100.0f);
}

OrthographicCamera& OrthographicCamera::SetPosition(const glm::vec3& position) noexcept
{
    m_Position = position;
    OrthographicCamera::UpdateData();

    return *this;
}

OrthographicCamera& OrthographicCamera::SetRotation(float rotation) noexcept
{
    m_Rotation = rotation;
    OrthographicCamera::UpdateData();

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

void OrthographicCamera::UpdateData()
{
    glm::mat4 transform{ glm::identity<glm::mat4>() };
    transform = glm::translate(transform, m_Position);
    transform = glm::rotate(transform, glm::radians(m_Rotation), glm::vec3(0.0f, 0.0f, 1.0f));

    m_ViewMatrix = glm::inverse(transform);
}

RENDERER_CODE_END
