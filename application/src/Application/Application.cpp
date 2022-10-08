#include "Application.hpp"

#include <filesystem>
#include <spdlog/spdlog.h>

#include <GLAD/glad.h>

#include "Utility/Checker.hpp"

Application::Application(const ApplicationProps& props) noexcept
    : m_Window{ std::make_unique<Window>(props.Name, props.WindowSize) },
      m_ImGuiContext{ std::make_unique<ImGuiBuildContext>() }
{}

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
        { [this]() { return m_ImGuiContext->Initialize(m_Window); }, "Failed to initialize ImGui context!", },
    })) return false;

    m_CustomShader = Renderer::Shader::Create({
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

    glGenTextures(1, &m_TextureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, m_TextureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0u);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TextureColorbuffer, 0);

    glGenRenderbuffers(1, &m_Renderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, m_Renderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        spdlog::critical("Framebuffer is not complete!");
        return false;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0u);

    m_Vertices = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f,
    };

    glGenVertexArrays(1, &m_VertexArray);
    glGenBuffers(1, &m_VertexBuffer);
    glBindVertexArray(m_VertexArray);

    glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_Vertices.size(), m_Vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return true;
}

void Application::Run() noexcept
{
    while (m_Window->IsOpen())
    {
        m_Window->OnUpdate();

        glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer);
        glEnable(GL_DEPTH_TEST);

        glViewport(0, 0, 800, 600);
        glClearColor(0.2f, 0.4f, 0.6f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        Application::OnRenderViewport();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDisable(GL_DEPTH_TEST);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        m_ImGuiContext->PreRender();
        Application::OnRenderImGui();
        m_ImGuiContext->PostRender();
    }
}

void Application::Shutdown() noexcept
{
    m_ImGuiContext->Shutdown();
}

void Application::OnRenderViewport() noexcept
{
    m_CustomShader->Bind();
    glBindVertexArray(m_VertexArray);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void Application::OnRenderImGui() noexcept
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

    // ImGui::ShowDemoWindow();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::Begin("Viewport");
    ImVec2 wsize{ ImGui::GetWindowSize() };
    ImGui::Image(reinterpret_cast<ImTextureID>(m_TextureColorbuffer), wsize, ImVec2(0, 1), ImVec2(1, 0));
    ImGui::End();
    ImGui::PopStyleVar();

    ImGui::Begin("Scene");

    const auto& shaderInfo{ m_CustomShaderData.ShaderData };

    static std::size_t contentIndex{ 0u };
    static bool openPopup{ false };

    ImGuiTableFlags tableFlags = ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableRowFlags_Headers;
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

    ImGui::Text("Viewport Size: (%f, %f)", wsize.x, wsize.y);

    ImGui::End();
}
