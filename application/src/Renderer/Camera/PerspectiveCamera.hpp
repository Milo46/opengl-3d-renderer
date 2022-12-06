#pragma once

#include "Renderer/RendererCore.hpp"
#include "Renderer/Camera/Camera.hpp"

RENDERER_CODE_BEGIN

struct PerspectiveProjection
{

};

class PerspectiveCamera : public Camera
{
public:

public:
    virtual const glm::mat4& GetViewMatrix() const override;
    virtual const glm::mat4& GetProjectionMatrix() const override;

private:
    void UdpateData();

private:
    glm::mat4 m_ViewMatrix{};
    glm::mat4 m_ProjectionMatrix{};

    glm::vec3 m_Position{};
    glm::vec3 m_Front{};
    glm::vec3 m_Up{};
    glm::vec3 m_Right{};
    glm::vec3 m_WorldUp{};

    float m_Yaw  { c_Yaw   };
    float m_Pitch{ c_Pitch };
    float m_Roll { c_Roll  };
    float m_Zoom { c_Zoom  };
};

RENDERER_CODE_END
