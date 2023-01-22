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

// class RendererInstance;

// class Scene
// {
// public:
//     virtual bool OnInitialize() { return true; }

//     virtual void OnUpdate(const Timestamp&) noexcept = 0;
//     virtual void OnRender() noexcept = 0;

// protected:
//     explicit Scene(Renderer::RendererInstance* renderer, Renderer::Camera* camera)
//         : m_RendererInstance{ renderer }, m_CameraInstance{ camera } {}

//     inline decltype(m_CameraInstance) GetCamera() { return m_CameraInstance; }

// private:
//     std::unique_ptr<Renderer::RendererInstance> m_RendererInstance{};
//     std::shared_ptr<Renderer::Camera> m_CameraInstance{};
// };

// class UserScene : public Scene
// {
// public:
//     explicit UserScene()
//         : Scene{
//             new Renderer::Renderer2D{},
//             new Renderer::PerspectiveCamera{}
//         } {}
    
//     virtual bool OnInitialize() noexcept override
//     {
//         m_Camera = Scene::GetCamera();
//         if (!m_Camera.get()) return false;

//         return true;
//     }

//     virtual void OnUpdate(const Timestamp& timestamp) noexcept override
//     {
//         glm::vec3 newPosition{};
//         newPosition.x = m_OrbitRadius * sin(timestamp.TotalTime);
//         newPosition.z = m_OrbitRadius * cos(timestamp.TotalTime);

//         m_Camera
//             ->SetPosition(newPosition)
//             ->SetLookDirection(glm::vec3(0.0f));
//     }

//     virtual void OnRender() noexcept override
//     {
//         Renderer::RenderCommand::SetClearColor({ 0.2f, 0.4f, 0.6f, 1.0f, });
//         Renderer::RenderCommand::Clear();

//         const glm::vec3& rectangleColor1{ .3f, .5f, .0f, };
//         const glm::vec3& rectangleColor2{ 1.f, .3f, .3f, };

//         Renderer::Renderer2D::DrawPlane({ 1.0f, 1.0f, 0.0f, }, { 0.0f, 0.0f, 0.1f, }, rectangleColor1 + rectangleColor2);
//         Renderer::Renderer2D::DrawPlane({ 0.5f, 0.5f, 0.0f, }, { 0.5f, 0.5f, 0.0f, }, rectangleColor1);
//         Renderer::Renderer2D::DrawPlane({ 0.75f, 0.75f, 0.0f, }, { 0.0f, 0.25f, -0.1f, }, rectangleColor2);
//     }

// private:
//     std::shared_ptr<Renderer::PerspectiveCamera> m_Camera{};
//     const float m_OrbitRadius{ 2.0f };
// };

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

    Renderer::PerspectiveCamera m_Camera;
    float m_UpdateAspectRatio{ 1.0f };

    glm::vec2 m_ViewportPosition{};
    glm::vec2 m_ViewportSize{};

    bool m_WireframeMode{ true };

    std::shared_ptr<Renderer::Framebuffer> m_Framebuffer{};

    Renderer::ShaderDataExtractor m_ShaderData{};
};

std::unique_ptr<Application> CreateApplication() noexcept;
