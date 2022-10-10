#pragma once

#include "Window/Window.hpp"
#include "Logger/Logger.hpp"
#include "Renderer/Renderer.hpp"

#include "ImGui/ImGuiContext.hpp"

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
    void OnUpdate(const float& deltaTime) noexcept;
    void OnRenderViewport() noexcept;
    void OnRenderImGui(ImGuiIO& io, const float& deltaTime) noexcept;

    void PanelViewport();
    void PanelShader();

private:
    std::unique_ptr<Window> m_Window;
    std::unique_ptr<ImGuiBuildContext> m_ImGuiContext;

    std::size_t m_TriangleCount{ 0u };
    bool m_WireframeMode{ false };

    std::shared_ptr<Renderer::Shader> m_CustomShader{};
    std::shared_ptr<Renderer::VertexArray> m_VertexArray{};
    Renderer::ShaderDataExtractor m_CustomShaderData{};

    Renderer::RendererID m_Framebuffer{ 0u };
    Renderer::RendererID m_TextureColorbuffer{ 0u };
    Renderer::RendererID m_Renderbuffer{ 0u };
};
