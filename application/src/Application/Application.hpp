#pragma once

#include "Renderer/Shader.hpp"
#include "Renderer/Texture2D.hpp"
#include "Renderer/VertexArray.hpp"

#include "Window/Window.hpp"
#include "Logger/Logger.hpp"
#include "Renderer/Renderer.hpp"

#include "ImGui/ImGuiContext.hpp"

#include "Framebuffer.hpp"

struct Timestamp // or Timepoint? Help.
{
    float TotalTime{};
    float DeltaTime{};
};

struct ApplicationProps
{
    const std::string_view Name;
    const glm::ivec2 WindowSize;
};

class Application
{
public:
    explicit Application(const ApplicationProps& props) noexcept;
    ~Application();

    bool Initialize() noexcept;
    void Run() noexcept;
    void Shutdown() noexcept;

private:
    void OnUpdate(const Timestamp& timestamp) noexcept;
    void OnRenderViewport() noexcept;
    void OnRenderImGui(ImGuiIO& io, const Timestamp& timestamp) noexcept;

    void PanelViewport(ImGuiIO& io, const Timestamp& timestamp);
    void PanelShader(ImGuiIO& io, const Timestamp& timestamp);

private:
    std::unique_ptr<Window> m_Window;
    std::unique_ptr<ImGuiBuildContext> m_ImGuiContext;
    Timestamp m_Timestamp{};

    Renderer::PerspectiveCamera m_Camera;
    float m_UpdateAspectRatio{ 1.0f };

    glm::vec2 m_ViewportPosition{};
    glm::vec2 m_ViewportSize{};

    std::size_t m_TriangleCount{ 0u };
    bool m_WireframeMode{ true };

    std::shared_ptr<Renderer::Framebuffer> m_Framebuffer{};

    Renderer::ShaderDataExtractor m_ShaderData{};
};
