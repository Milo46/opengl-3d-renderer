#pragma once

#include "Renderer/Backend/Shader.hpp"
#include "Renderer/Backend/Texture2D.hpp"
#include "Renderer/Backend/VertexArray.hpp"
#include "Renderer/Backend/Framebuffer.hpp"

#include "Window/Window.hpp"
#include "Logger/Logger.hpp"
#include "Renderer/Renderer.hpp"

#include "ImGui/ImGuiContext.hpp"

#include "OBJLoader.hpp"

struct Timestamp // or Timepoint? Help.
{
    float TotalTime{};
    float DeltaTime{};
};

class Scene
{
public:
    virtual bool OnInitialization() { return true; }

    virtual void OnUpdate(const Timestamp& timestamp) noexcept = 0;
    virtual void OnRender() noexcept = 0;

    virtual void OnImGuiRender(ImGuiIO& io, const Timestamp& timestamp) noexcept {};

public:
    virtual ~Scene() = default;

protected:
    inline Scene(std::unique_ptr<Window>& windowRef)
        : m_WindowRef{ windowRef } {}

    inline std::unique_ptr<Window>& GetWindow() noexcept { return m_WindowRef; }

private:
    std::unique_ptr<Window>& m_WindowRef;
};

struct ApplicationProps
{
    std::string_view Name;
    glm::ivec2 WindowSize;
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

    bool m_WireframeMode{ true };

    std::shared_ptr<Renderer::Framebuffer> m_Framebuffer{};
    std::shared_ptr<Scene> m_ActiveScene{};
};

std::unique_ptr<Application> CreateApplication() noexcept;
