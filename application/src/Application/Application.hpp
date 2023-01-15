#pragma once

#include <stack>

#include "Renderer/Shader.hpp"
#include "Renderer/Texture2D.hpp"
#include "Renderer/VertexArray.hpp"

#include "Window/Window.hpp"
#include "Logger/Logger.hpp"
#include "Renderer/Renderer.hpp"

#include "ImGui/ImGuiContext.hpp"

// class State
// {
//     friend class Application;

// protected:
//     explicit State(const std::string_view name);

//     virtual void OnInitialize()  = 0;
//     virtual void OnDestruction() = 0;

//     virtual void OnUpdate() = 0;
//     virtual void OnRender(const float& deltaTime) = 0;
// };

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

    // std::unique_ptr<Renderer> m_Renderer;
    // std::stack<State*> m_States{};

    std::size_t m_TriangleCount{ 0u };
    bool m_WireframeMode{ true };

    Renderer::RendererID m_Framebuffer{ 0u };
    Renderer::RendererID m_Renderbuffer{ 0u };
    std::shared_ptr<Renderer::Texture2D> m_TextureColorbuffer{ 0u };

    Renderer::ShaderDataExtractor m_ShaderData{};
};
