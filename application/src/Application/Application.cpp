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
      m_Camera      { { -1.0f, 1.0f, -1.0f, 1.0f, },                         }
{
    // m_Window->AddKeybinds({
        // { WindowAction::Miximize, { GLFW_KEY_F10, }, },
        // { WindowAction::Iconify,  { GLFW_KEY_F9,  }, },
    // });

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

    m_CustomShader = Renderer::Create<Renderer::Shader>({
        .Sources = {
            { Renderer::ShaderType::Vertex,   { "assets/shaders/vertex.glsl",   }, },
            { Renderer::ShaderType::Fragment, { "assets/shaders/fragment.glsl", }, },
        },
    });
    spdlog::info("Custom shader compilation result: {}", m_CustomShader->Compile());
    spdlog::info("Custom shader linking result: {}", m_CustomShader->Link());

    m_CustomShaderData.Extract(m_CustomShader);

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

    std::vector<float> vertices{ 0.5f,  0.5f, 0.0f, 0.5f, -0.5f, 0.0f, -0.5f, -0.5f, 0.0f, -0.5f,  0.5f, 0.0f, };
    std::vector<unsigned int> indices{ 0u, 1u, 3u, 1u, 2u, 3u, };

    auto vertexBuffer{ Renderer::Create<Renderer::VertexBuffer, Renderer::Extension::VertexBufferPropsVector>({
        .VectorData = vertices,
        .Layout = {
            { Renderer::LayoutDataType::Float3, "a_Position", },
        },
    }) };

    auto indexBuffer{ Renderer::Create<Renderer::IndexBuffer, Renderer::Extension::IndexBufferPropsVector>({
        .VectorData = indices,
    }) };

    m_RectangleVA = Renderer::Create<Renderer::VertexArray>({
        .VertexBuffer = vertexBuffer,
        .IndexBuffer  = indexBuffer,
    });

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

        m_TriangleCount = 0u;

        glPolygonMode(GL_FRONT_AND_BACK, m_WireframeMode ? GL_LINE : GL_FILL);

        Application::OnUpdate(deltaTime);
        Application::OnRenderViewport();

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

    // m_CustomShader->Bind();
    // m_CustomShader->SetUniform("u_Time", static_cast<float>(glfwGetTime()));
    // m_CustomShader->SetUniform("u_Color", glm::vec3{ 0.8f, 0.2f, 0.2f, });

    // m_RectangleVA->Bind();
    // glDrawElements(GL_TRIANGLES, m_RectangleVA->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
    // m_TriangleCount += m_RectangleVA->GetIndexBuffer()->GetCount() / 3.0f;

    Renderer::Renderer2D::BeginScene(m_Camera);
    Renderer::Renderer2D::DrawRectangle({ 1.0f, 1.0f, 0.0f, }, { 0.0f, 0.0f, 0.0f, }, { 0.3f, 0.5f, 0.0f, });
    // Renderer::Renderer2D::DrawRectangle({ 0.75f, 0.75f, 0.0f, }, { 100.0f, 0.25f, 0.0f, }, { 1.0f, 0.3f, 0.3f, });
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
    Application::PanelShader();
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

void Application::PanelShader()
{
    ImGui::Begin("Shader Preview");

    const auto& shaderInfo{ m_CustomShaderData.ShaderData };
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
