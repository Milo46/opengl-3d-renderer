#include "PerspectiveCamera.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

RENDERER_CODE_BEGIN

PerspectiveCamera::PerspectiveCamera(const PerspectiveProjection& props)
    : m_Position{ props.Position },
      m_WorldUp{ 0.0f, 1.0f, 0.0f, },
      m_ProjectionMatrix{ glm::perspective(props.Fov, props.Ratio, props.Near, props.Far) }
{
    PerspectiveCamera::UpdateData();
}

void PerspectiveCamera::OnUpdate(const std::unique_ptr<Window>& window)
{
    PerspectiveCamera::OnUpdate(window->GetAspectRatio());
}

void PerspectiveCamera::OnUpdate(float aspectRatio)
{
    const auto nearPlane{ 0.1f };
    const auto farPlane{ 1000.0f };

    m_ProjectionMatrix = glm::perspective(glm::radians(m_Zoom), aspectRatio, nearPlane, farPlane);
    m_ViewMatrix       = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
}

const glm::mat4& PerspectiveCamera::GetViewMatrix() const
{
    return m_ViewMatrix;
}

const glm::mat4& PerspectiveCamera::GetProjectionMatrix() const
{
    return m_ProjectionMatrix;
}

PerspectiveCamera& PerspectiveCamera::SetPosition(const glm::vec3& position)
{
    m_Position = position;
    PerspectiveCamera::UpdateData();
    return *this;
}

const glm::vec3& PerspectiveCamera::GetPosition() const
{
    return m_Position;
}

void PerspectiveCamera::UpdateData()
{
    m_Front = glm::normalize(glm::vec3{
       cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch)),
       sin(glm::radians(m_Pitch)),
       sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch)),
    });

    m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
    m_Up    = glm::normalize(glm::cross(m_Right, m_Front));

    m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
    // m_ViewMatrix = glm::lookAt(m_Position, glm::vec3(0.0f), m_Up);
}

RENDERER_CODE_END
