#pragma once

#include "Renderer/RendererCore.hpp"
#include "Renderer/Camera/Camera.hpp"

RENDERER_CODE_BEGIN

struct OrthographicProjection
{
    float Left{ 0.0f };
    float Right{ 0.0f };
    float Bottom{ 0.0f };
    float Top{ 0.0f };
};

class OrthographicCamera : public Camera
{
public:
    OrthographicCamera(const OrthographicProjection& projection);
    void SetProjection(const OrthographicProjection& projection);

    OrthographicCamera& SetPosition(const glm::vec3& position) noexcept;
    OrthographicCamera& SetRotation(float rotation) noexcept;

    inline const auto& GetPosition() const noexcept { return m_Position; }
    inline const auto& GetRotation() const noexcept { return m_Rotation; }

public:
    virtual const glm::mat4& GetViewMatrix() const override;
    virtual const glm::mat4& GetProjectionMatrix() const override;

private:
    void UpdateData();

private:
    glm::mat4 m_ViewMatrix{};
    glm::mat4 m_ProjectionMatrix{};

    glm::vec3 m_Position{};
    float m_Rotation{ 0.0f };
};

RENDERER_CODE_END
