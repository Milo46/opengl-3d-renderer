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

const glm::mat4& PerspectiveCamera::GetViewMatrix() const
{
    return m_ViewMatrix;
}

const glm::mat4& PerspectiveCamera::GetProjectionMatrix() const
{
    return m_ProjectionMatrix;
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
}

RENDERER_CODE_END
