#pragma once

#include "Camera.hpp"

NAMESPACE_BEGIN(Renderer)

struct OrthographicProjection
{
    float Left  { -1.0f };
    float Right {  1.0f };
    float Bottom{ -1.0f };
    float Top   {  1.0f };

    static glm::mat4 Calculate(const OrthographicProjection& projection);
};

class OrthographicCamera : public Camera
{
public:
    explicit OrthographicCamera(const OrthographicProjection& projection = {});

    OrthographicCamera& SetPosition(const glm::vec3& position) noexcept;
    OrthographicCamera& SetRotation(const float rotation) noexcept;

    inline const auto& GetPosition() const noexcept { return m_Position; }
    inline const auto& GetRotation() const noexcept { return m_Rotation; }

public:
    virtual const glm::mat4& GetViewMatrix() const override;
    virtual const glm::mat4& GetProjectionMatrix() const override;

private:
    void RecalculateViewMatrix();

private:
    glm::mat4 m_ViewMatrix{};
    glm::mat4 m_ProjectionMatrix{};

    glm::vec3 m_Position{};
    float m_Rotation{ 0.0f };
};

NAMESPACE_END(Renderer)
