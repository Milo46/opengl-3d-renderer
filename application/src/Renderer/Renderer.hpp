#pragma once

#include "Renderer/RendererCore.hpp"
#include "Renderer/RenderCommand.hpp"

#include <glm/glm.hpp>

RENDERER_CODE_BEGIN

class OrthographicCamera
{
public:
    OrthographicCamera(float left, float right, float bottom, float top);
    void SetProjection(float left, float right, float bottom, float top);

    OrthographicCamera& SetPosition(const glm::vec3& position) noexcept;
    OrthographicCamera& SetRotation(const float rotation) noexcept;

    inline const auto& GetPosition() const noexcept { return m_Position; }
    inline const auto& GetRotation() const noexcept { return m_Rotation; }
    inline const auto& GetProjectionMatrix() const noexcept { return m_ProjectionMatrix; }
    inline const auto& GetViewMatrix() const noexcept { return m_ViewMatrix; }

private:
    void RecalculateData() noexcept;

private:
    glm::mat4 m_ProjectionMatrix{};
    glm::mat4 m_ViewMatrix{};

    glm::vec3 m_Position{};
    float m_Rotation{ 0.0f };
};

class Renderer2D
{
public:
    static bool Initialize();
    static void Shutdown();

    static void BeginScene(const OrthographicCamera& camera);
    static void EndScene();

    static void DrawRectangle(const glm::vec3& size, const glm::vec3& position, const glm::vec3& color = glm::vec3(1.0f));
};

RENDERER_CODE_END
