#include "PerspectiveCamera.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

NAMESPACE_BEGIN(Renderer)

PerspectiveCamera::PerspectiveCamera(const PerspectiveProjection& projection)
    : m_Position{ projection.Position },
      m_WorldUp{ 0.0f, 1.0f, 0.0f, },
      m_ProjectionMatrix{ glm::perspective(projection.Fov, projection.Ratio, projection.Near, projection.Far) },
      m_NearPlane{ projection.Near },
      m_FarPlane{ projection.Far }
{
    PerspectiveCamera::RecalculateViewMatrix();
}

void PerspectiveCamera::OnUpdate(float aspectRatio)
{
    m_ProjectionMatrix = glm::perspective(glm::radians(m_Zoom), aspectRatio, m_NearPlane, m_FarPlane);
    m_ViewMatrix       = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
}

PerspectiveCamera& PerspectiveCamera::SetPosition(const glm::vec3& position) noexcept
{
    m_Position = position;
    PerspectiveCamera::RecalculateViewMatrix();

    return *this;
}

PerspectiveCamera& PerspectiveCamera::SetLookDirection(const glm::vec3& direction) noexcept
{
    m_ViewMatrix = glm::lookAt(m_Position, direction, m_Up);

    return *this;
}

const glm::mat4& PerspectiveCamera::GetViewMatrix() const
{
    return m_ViewMatrix;
}

const glm::mat4& PerspectiveCamera::GetProjectionMatrix() const
{
    return m_ProjectionMatrix;
}

void PerspectiveCamera::RecalculateViewMatrix()
{
    m_Front = glm::normalize(glm::vec3{
       cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch)),
       sin(glm::radians(m_Pitch)),
       sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch)),
    });

    m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
    m_Up = glm::normalize(glm::cross(m_Right, m_Front));

    m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
}

NAMESPACE_END(Renderer)
