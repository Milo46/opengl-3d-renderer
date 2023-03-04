#include "Application.hpp"

#include "Filesystem/Filesystem.hpp"
#include "Utility/Checker.hpp"
#include "Logger/Logger.hpp"

#include "Renderer/RenderCommand.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

Application::Application(const ApplicationProps& props) noexcept
    : m_Window      { std::make_unique<Window>(props.Name, props.WindowSize) },
      m_ImGuiContext{ std::make_unique<ImGuiBuildContext>()                  }
{
    m_Window->AddKeybinds({
        { WindowAction::Maximize, { GLFW_KEY_F10, }, },
        { WindowAction::Minimize, { GLFW_KEY_F9,  }, },
    });
    m_Window->SetVSync(true);
}

Application::~Application()
{
    m_ImGuiContext->Shutdown();
}

bool Application::OnInit()
{
    if (!Checker::PerformSequence(spdlog::level::critical, {
        { [this]() { return Filesystem::Initialize();             }, },
        { [this]() { return Logger::Initialize();                 }, },
        { [this]() { return m_Window->OnInit();                   }, },
        { [this]() { return m_ImGuiContext->Initialize(m_Window); }, },
        { [this]() { return OnInitApp();                          }, },
    })) return false;

    return true;
}

void Application::Run()
{
    while (m_Window->IsOpen())
    {
        const float currentTotalTime{ static_cast<float>(glfwGetTime()) };
        m_Timestamp.DeltaTime = currentTotalTime - m_Timestamp.TotalTime;
        m_Timestamp.TotalTime = currentTotalTime;

        m_Window->OnUpdate();

        OnUpdate(m_Timestamp);

        Renderer::RenderCommand::SetClearColor({ 0.2f, 0.4f, 0.6f, 1.0f, });
        Renderer::RenderCommand::Clear();
        OnRender();

        m_ImGuiContext->PreRender();
        OnImGuiRender(m_ImGuiContext->GetIO());
        m_ImGuiContext->PostRender();
    }
}

bool Application::OnInitApp()
{
    return true;
}
