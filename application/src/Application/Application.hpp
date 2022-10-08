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

    void OnRenderViewport() noexcept;
    void OnRenderImGui() noexcept;

private:
    std::unique_ptr<Window> m_Window;
    std::unique_ptr<ImGuiBuildContext> m_ImGuiContext;

    std::shared_ptr<Renderer::Shader> m_CustomShader{};
    Renderer::ShaderDataExtractor m_CustomShaderData{};

    Renderer::RendererID m_Framebuffer{ 0u };
    Renderer::RendererID m_TextureColorbuffer{ 0u };
    Renderer::RendererID m_Renderbuffer{ 0u };

    std::vector<float> m_Vertices{};
    Renderer::RendererID m_VertexBuffer{ 0u };
    Renderer::RendererID m_VertexArray{ 0u };
};
