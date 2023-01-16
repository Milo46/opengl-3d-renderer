#pragma once

#include "Renderer/RendererCore.hpp"
#include "Renderer/Camera/Camera.hpp"

#include "Window/Window.hpp"

RENDERER_CODE_BEGIN

struct PerspectiveProjection
{
    glm::vec3 Position{ 0.0f, 0.0f, 0.0f, };
    glm::vec3 Direction{ 0.0f, 0.0f, 1.0f, };

    float Fov{ 45.0f };
    float Ratio{ 1.0f };
    float Near{ 0.1f };
    float Far{ 1000.0f };
};

class PerspectiveCamera : public Camera
{
public:
    inline static const float c_Yaw{ -90.0f };
    inline static const float c_Pitch{ 0.0f };
    inline static const float c_Roll{ 0.0 };
    inline static const float c_Zoom{ 45.0f };

public:
    explicit PerspectiveCamera(const PerspectiveProjection& props);

    void OnUpdate(const std::unique_ptr<Window>& window);
    void OnUpdate(float aspectRatio);

public:
    virtual const glm::mat4& GetViewMatrix() const override;
    virtual const glm::mat4& GetProjectionMatrix() const override;

    const glm::vec3& GetPosition() const;

public:
    PerspectiveCamera& SetPosition(const glm::vec3& position);
    PerspectiveCamera& SetLookDirection(const glm::vec3& direction);

private:
    void UpdateData();

private:
    glm::mat4 m_ViewMatrix{};
    glm::mat4 m_ProjectionMatrix{};

    float m_NearPlane{};
    float m_FarPlane{};

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
