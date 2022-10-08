#include "Application.hpp"

#include <filesystem>
#include <spdlog/spdlog.h>

#include <GLAD/glad.h>

struct CheckInstruction
{
    const std::function<bool()> Function;
    const std::string_view OnError;
};

class Checker
{
public:
    // using CheckInstruction = std::tuple<std::function<bool>(), std::string_view>;
    using CheckSequence    = std::vector<CheckInstruction>;

public:
    static bool PerformSequence(spdlog::level::level_enum severity, const CheckSequence& sequence)
    {
        for (const auto& [check, onError] : sequence)
        {
            if (check()) continue;
            spdlog::log(severity, onError);
            return false;
        }
        return true;
    }
};

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

    return true;
}

void Application::Run() noexcept
{
    while (m_Window->IsOpen())
    {
        m_Window->OnUpdate();

        glClearColor(0.2f, 0.4f, 0.6f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        m_ImGuiContext->PreRender();
        Application::RenderImGui();
        m_ImGuiContext->PostRender();
    }
}

void Application::Shutdown() noexcept
{
    m_ImGuiContext->Shutdown();
}

void Application::RenderImGui() noexcept
{
    static ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus |
        ImGuiWindowFlags_NoBackground;

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

    ImGui::End();
}
