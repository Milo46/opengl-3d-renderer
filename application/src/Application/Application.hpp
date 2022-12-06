#pragma once

#include <stack>

#include "Renderer/Shader.hpp"
#include "Renderer/Texture2D.hpp"
#include "Renderer/VertexArray.hpp"

#include "Window/Window.hpp"
#include "Logger/Logger.hpp"
#include "Renderer/Renderer.hpp"

#include "ImGui/ImGuiContext.hpp"

class State
{
    friend class Application;

protected:
    explicit State(const std::string_view name);

    virtual void OnInitialize()  = 0;
    virtual void OnDestruction() = 0;

    virtual void OnUpdate() = 0;
    virtual void OnRender(const float& deltaTime) = 0;
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
    void OnUpdate(const float& deltaTime) noexcept;
    void OnRenderViewport() noexcept;
    void OnRenderImGui(ImGuiIO& io, const float& deltaTime) noexcept;

    void PanelViewport(ImGuiIO& io, const float& deltaTime);
    void PanelShader();

private:
    std::unique_ptr<Window> m_Window;
    std::unique_ptr<ImGuiBuildContext> m_ImGuiContext;
    Renderer::OrthographicCamera m_Camera;

    // std::unique_ptr<Renderer> m_Renderer;
    // std::stack<State*> m_States{};

    std::size_t m_TriangleCount{ 0u };
    bool m_WireframeMode{ false };

    Renderer::RendererID m_Framebuffer{ 0u };
    Renderer::RendererID m_Renderbuffer{ 0u };
    std::shared_ptr<Renderer::Texture2D> m_TextureColorbuffer{ 0u };

    std::shared_ptr<Renderer::VertexArray> m_RectangleVA{};
    std::shared_ptr<Renderer::Shader> m_CustomShader{};
    Renderer::ShaderDataExtractor m_CustomShaderData{};
};
