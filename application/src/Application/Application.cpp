#include "Application.hpp"

#include <filesystem>
#include <spdlog/spdlog.h>

#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#include "Utility/Checker.hpp"

#include "Extensions/Renderer/BuffersVectorProps.hpp"

Application::Application(const ApplicationProps& props) noexcept
    : m_Window      { std::make_unique<Window>(props.Name, props.WindowSize) },
      m_ImGuiContext{ std::make_unique<ImGuiBuildContext>()                  },
      m_PerspectiveCamera{ { { 0.0f, 0.0f, -2.0f, } } }
{
    m_Camera.SetPosition({ 0.0f, 0.0f, 0.0f, });

    m_Window->AddKeybinds({
        { WindowAction::Miximize, { GLFW_KEY_F10, }, },
        { WindowAction::Iconify,  { GLFW_KEY_F9,  }, },
    });

    m_Window->SetVSync(true);
}

Application::~Application()
{
    Application::Shutdown();
}

bool Application::Initialize() noexcept
{
    const auto rootFilepath{ std::filesystem::path{ __FILE__ }.append("../../../") };
    std::filesystem::current_path(rootFilepath);

    if (!Checker::PerformSequence(spdlog::level::critical, {
        { [this]() { return Logger::Initialize();                 }, "Failed to initialize the logger!",    },
        { [this]() { return m_Window->Initialize();               }, "Failed to initialize the window!",    },
        { [this]() { return Renderer::Renderer2D::Initialize();   }, "Failed to initialize the renderer!",  },
        // { [this]() { return m_Renderer->Initialize();             }, "Failed to initialize the renderer!",  },
        { [this]() { return m_ImGuiContext->Initialize(m_Window); }, "Failed to initialize ImGui context!", },
    })) return false;

    m_ShaderData.Extract(Renderer::Renderer2D::GetFlatShader());

    glGenFramebuffers(1, &m_Framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer);

    m_TextureColorbuffer = Renderer::Create<Renderer::Texture2D>({
        .Size = { 800u, 600u, },
    });

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TextureColorbuffer->GetHandle(), 0);

    glGenRenderbuffers(1, &m_Renderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, m_Renderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_TextureColorbuffer->GetWidth(), m_TextureColorbuffer->GetHeight());
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        spdlog::critical("Framebuffer is not complete!");
        return false;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0u);

    return true;
}

void Application::Run() noexcept
{
    static float deltaTime{ 0.0f };
    static float lastFrame{ 0.0f };

    while (m_Window->IsOpen())
    {
        float currentFrame{ static_cast<float>(glfwGetTime()) };
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        m_Window->OnUpdate();

        glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer);
        glEnable(GL_DEPTH_TEST);

        glPolygonMode(GL_FRONT_AND_BACK, m_WireframeMode ? GL_LINE : GL_FILL);

        Application::OnUpdate(deltaTime);
        Application::OnRenderViewport();

        m_TriangleCount = Renderer::Renderer2D::GetTriangleCount();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDisable(GL_DEPTH_TEST);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        m_ImGuiContext->PreRender();
        Application::OnRenderImGui(m_ImGuiContext->GetIO(), deltaTime);
        m_ImGuiContext->PostRender();
    }
}

void Application::Shutdown() noexcept
{
    m_ImGuiContext->Shutdown();
}

void Application::OnUpdate(const float& deltaTime) noexcept
{
}

void Application::OnRenderViewport() noexcept
{
    Renderer::RenderCommand::SetViewport(0, 0, 800, 600);
    Renderer::RenderCommand::SetClearColor({ 0.2f, 0.4f, 0.6f, 1.0f, });
    Renderer::RenderCommand::Clear();

    const glm::vec3& rectangleColor1{ .3f, .5f, .0f, };
    const glm::vec3& rectangleColor2{ 1.f, .3f, .3f, };

    // TODO: Make perspective camera work, maybe it can fix the Z-buffer issue.
    Renderer::Renderer2D::BeginScene(&m_Camera);
    // Renderer::Renderer2D::BeginScene(&m_PerspectiveCamera);

    // TODO: Z-buffer doesn't change anything in rendering process, but it should! Make Z-buffer matter.
    Renderer::Renderer2D::DrawRectangle({ 1.0f, 1.0f, 0.0f, }, { 0.0f, 0.0f, 0.0f, }, rectangleColor1 + rectangleColor2);

    Renderer::Renderer2D::DrawRectangle({ 0.5f, 0.5f, 0.0f, }, { 0.5f, 0.5f, 0.0f, }, rectangleColor1);
    Renderer::Renderer2D::DrawRectangle({ 0.75f, 0.75f, 0.0f, }, { 0.0f, 0.25f, 0.0f, }, rectangleColor2);
    Renderer::Renderer2D::EndScene();
}

void Application::OnRenderImGui(ImGuiIO& io, const float& deltaTime) noexcept
{
    static ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground;

    const auto viewport{ ImGui::GetMainViewport() };
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockingWindow", nullptr, windowFlags);
    ImGui::PopStyleVar(3);

    ImGuiID dockspaceID{ ImGui::GetID("WindowDockspace") };
    ImGui::DockSpace(dockspaceID, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
    ImGui::End();

    ImGui::ShowDemoWindow();

    Application::PanelViewport(io, deltaTime);
    Application::PanelShader(io, deltaTime);
}

void Application::PanelViewport(ImGuiIO& io, const float& deltaTime)
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoScrollbar /*| ImGuiWindowFlags_MenuBar*/);
    // if (ImGui::BeginMenuBar())
    // {
    //     if (ImGui::BeginMenu("View"))
    //     {
    //         ImGui::MenuItem("Show Framerates", nullptr, true);
    //         ImGui::MenuItem("Show Rendered Triangles", nullptr, true);
    //         ImGui::MenuItem("Toggle VSync", nullptr, true);
    //         ImGui::MenuItem("Toggle Wireframe", nullptr, true);

    //         ImGui::Checkbox("Wireframe Mode", &m_WireframeMode);
    //         ImGui::EndMenu();
    //     }
    //     ImGui::EndMenuBar();
    // }

    ImVec2 wsize{ ImGui::GetContentRegionAvail() };
    ImGui::Image(reinterpret_cast<ImTextureID>(m_TextureColorbuffer->GetHandle()), wsize, ImVec2(0, 1), ImVec2(1, 0));
    ImGui::SetCursorPos(ImVec2(5, 25)); ImGui::Text("Viewport Size: (%f, %f)", wsize.x, wsize.y);
    ImGui::SetCursorPos(ImVec2(5, 45)); ImGui::Text("FPS ImGui: %f", 1.0f / io.DeltaTime);
    ImGui::SetCursorPos(ImVec2(5, 65)); ImGui::Text("FPS Viewport: %f", 1.0f / deltaTime);
    ImGui::SetCursorPos(ImVec2(5, 85)); ImGui::Text("Triangles rendered: %d", m_TriangleCount);
    ImGui::SetCursorPos(ImVec2(5, 105));
    if (ImGui::Button("Toggle VSync"))
        m_Window->SetVSync(!m_Window->IsVSync());
    ImGui::SetCursorPos(ImVec2(5, 125));
    if (ImGui::Button("Toggle Wireframe"))
    {
        glPolygonMode(GL_FRONT_AND_BACK, m_WireframeMode ? GL_LINE : GL_FILL);
        m_WireframeMode = !m_WireframeMode;
    }
    ImGui::End();
    ImGui::PopStyleVar();
}

void Application::PanelShader(ImGuiIO& io, const float& deltaTime)
{
    ImGui::Begin("Shader Preview");

    const auto& shaderInfo{ m_ShaderData.ShaderData };
    static std::size_t contentIndex{ 0u };
    static bool openPopup{ false };

    static ImGuiTableFlags tableFlags = ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableRowFlags_Headers;
    if (ImGui::BeginTable("Program Shader Sources", 3, tableFlags))
    {
        ImGui::TableSetupColumn("Shader Type");
        ImGui::TableSetupColumn("Source Filepath");
        ImGui::TableSetupColumn("Preview");
        ImGui::TableHeadersRow();

        for (int i{ 0 }; i < shaderInfo.size(); ++i)
        {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("%s", shaderInfo[i].Type.data());
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%s", shaderInfo[i].Source.GetPath().data());
            ImGui::TableSetColumnIndex(2);
            if (ImGui::Button(std::string{ "Show source code!##" + std::to_string(i) }.data()))
            {
                ImGui::OpenPopup("Source Code");
                contentIndex = i;
                openPopup = true;
            }
        }

        if (ImGui::BeginPopupModal("Source Code", &openPopup))
        {
            ImGui::Text("%s", shaderInfo[contentIndex].Source.GetContent().data());
            ImGui::EndPopup();
        }

        ImGui::EndTable();
    }

    ImGui::End();
}
