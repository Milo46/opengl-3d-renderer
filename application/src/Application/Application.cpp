#include "Application.hpp"

#include <spdlog/spdlog.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Filesystem/Filesystem.hpp"
#include "Utility/Checker.hpp"

#include "Scene.hpp"

Application::Application(const ApplicationProps& props) noexcept
    : m_Window      { std::make_unique<Window>(props.Name, props.WindowSize) },
      m_ImGuiContext{ std::make_unique<ImGuiBuildContext>()                  }
{
    m_Window->AddKeybinds({
        { WindowAction::Miximize, { GLFW_KEY_F10, }, },
        { WindowAction::Iconify,  { GLFW_KEY_F9,  }, },
    });
    m_Window->SetVSync(true);

    m_ActiveScene = std::make_shared<UserScene>(m_Window);
}

Application::~Application()
{
    Application::Shutdown();
}

bool Application::Initialize() noexcept
{
    m_Framebuffer = Renderer::AllocateResource<Renderer::Framebuffer>({ .Size = { 1920, 1080, }, });

    if (!Checker::PerformSequence(spdlog::level::critical, {
        { [this]() { return Filesystem::Initialize();              }, "Failed to initialize the filesystem!",  },
        { [this]() { return Logger::Initialize();                  }, "Failed to initialize the logger!",      },
        { [this]() { return m_Window->Initialize();                }, "Failed to initialize the window!",      },
        { [this]() { return m_ImGuiContext->Initialize(m_Window);  }, "Failed to initialize ImGui context!",   },
        { [this]() { return m_Framebuffer->OnInitialize();         }, "Failed to initialize the framebuffer!", },
    })) return false;

    if (!m_ActiveScene->OnInitialization())
    {
        spdlog::error("Failed to initialize the Active Scene!");
        return false;
    }

    return true;
}

void Application::Run() noexcept
{
    while (m_Window->IsOpen())
    {
        const float currentTotalTime{ static_cast<float>(glfwGetTime()) };
        m_Timestamp.DeltaTime = currentTotalTime - m_Timestamp.TotalTime;
        m_Timestamp.TotalTime = currentTotalTime;

        /* I really hate to place experimental code here.
           It still only works if viewport is docked to the window, hopefully I'll find a better solution.
           And it disables the buttons inside the viewport, what? */
        // static bool holdingTheViewport{ false };
        // double xpos{}, ypos{};
        // glfwGetCursorPos(m_Window->GetNativeWindow(), &xpos, &ypos);
        // if (xpos >= m_ViewportPosition.x && xpos <= m_ViewportPosition.x + m_ViewportSize.x &&
        //     ypos >= m_ViewportPosition.y && ypos <= m_ViewportPosition.y + m_ViewportSize.y &&
        //     glfwGetMouseButton(m_Window->GetNativeWindow(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS &&
        //     holdingTheViewport == false)
        // {
        //     holdingTheViewport = true;
        //     spdlog::info("Cursor is holding the viewport!");

        //     glfwSetInputMode(m_Window->GetNativeWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        // }
        
        // if (glfwGetMouseButton(m_Window->GetNativeWindow(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE &&
        //     holdingTheViewport == true)
        // {
        //     holdingTheViewport = false;
        //     spdlog::info("Cursor released the viewport!");

        //     glfwSetInputMode(m_Window->GetNativeWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        // }
        // Here ends the bullshit.

        m_Window->OnUpdate();
        // m_Camera.OnUpdate(m_UpdateAspectRatio);

        // m_Framebuffer->Bind();
        // Renderer::RenderCommand::SetDepthTest(true);
        Application::OnUpdate(m_Timestamp);
        Application::OnRenderViewport();

        m_ImGuiContext->PreRender();
        m_ActiveScene->OnImGuiRender(m_ImGuiContext->GetIO(), m_Timestamp);
        m_ImGuiContext->PostRender();

        // m_Framebuffer->Unbind();
        // Renderer::RenderCommand::SetDepthTest(false);
        // Renderer::RenderCommand::SetViewport(0, 0, m_Window->GetSize().x, m_Window->GetSize().y);
        // Renderer::RenderCommand::SetClearColor(glm::vec4(1.0f));
        // Renderer::RenderCommand::Clear();
        // m_ImGuiContext->PreRender();
        // Application::OnRenderImGui(m_ImGuiContext->GetIO(), m_Timestamp);
        // m_ImGuiContext->PostRender();
    }
}

void Application::Shutdown() noexcept
{
    m_ImGuiContext->Shutdown();
}

void Application::OnUpdate(const Timestamp& timestamp) noexcept
{
    m_ActiveScene->OnUpdate(timestamp);
}

void Application::OnRenderViewport() noexcept
{
    // const auto& framebufferSize{ m_Framebuffer->GetSize() };
    // Renderer::RenderCommand::SetViewport(0, 0, framebufferSize.x, framebufferSize.y);

    Renderer::RenderCommand::SetClearColor({ 0.2f, 0.4f, 0.6f, 1.0f, });
    Renderer::RenderCommand::Clear();

    // const glm::vec3& rectangleColor1{ .3f, .5f, .0f, };
    // const glm::vec3& rectangleColor2{ 1.f, .3f, .3f, };

    // m_RendererContext->BeginScene(&m_Camera);
    // m_RendererContext->SetLightPosition(m_Camera.GetPosition());

    // // Renderer::Renderer2D::DrawPlane({ 1.0f, 1.0f, 0.0f, }, { 0.0f, 0.0f, 0.1f, }, rectangleColor1 + rectangleColor2);
    // // Renderer::Renderer2D::DrawPlane({ 0.5f, 0.5f, 0.0f, }, { 0.5f, 0.5f, 0.0f, }, rectangleColor1);
    // // Renderer::Renderer2D::DrawPlane({ 0.75f, 0.75f, 0.0f, }, { 0.0f, 0.25f, -0.1f, }, rectangleColor2);
    // // Renderer::Renderer2D::DrawPlane({ 1.0f, 1.0f, 0.0f, }, { 0.0f, 0.0f, 1.0f, }, { 90.0f, 0.0f, 0.0f, }, glm::vec3(1.0f));
    // // Renderer::Renderer2D::DrawArrays(m_TeapotVA, m_TeapotOBJData.size());

    // // m_RendererContext->DrawPlane({});

    // m_RendererContext->DrawArrays(m_TeapotVA, m_DiffuseMap, m_SpecularMap, m_EmissionMap, false);

    // m_RendererContext->EndScene();

    m_ActiveScene->OnRender();
}

void Application::OnRenderImGui(ImGuiIO& io, const Timestamp& timestamp) noexcept
{
//     static ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
//         ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
//         ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground;

//     const auto viewport{ ImGui::GetMainViewport() };
//     ImGui::SetNextWindowPos(viewport->WorkPos);
//     ImGui::SetNextWindowSize(viewport->WorkSize);
//     ImGui::SetNextWindowViewport(viewport->ID);

//     ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
//     ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
//     ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
//     ImGui::Begin("DockingWindow", nullptr, windowFlags);
//     ImGui::PopStyleVar(3);

//     ImGuiID dockspaceID{ ImGui::GetID("WindowDockspace") };
//     ImGui::DockSpace(dockspaceID, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
//     ImGui::End();

//     ImGui::ShowDemoWindow();

//     Application::PanelViewport(io, timestamp);
//     // Application::PanelShader(io, timestamp);
}

void Application::PanelViewport(ImGuiIO& io, const Timestamp& timestamp)
{
//     ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
//     ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoScrollbar /*| ImGuiWindowFlags_MenuBar*/);
//     // if (ImGui::BeginMenuBar())
//     // {
//     //     if (ImGui::BeginMenu("View"))
//     //     {
//     //         ImGui::MenuItem("Show Framerates", nullptr, true);
//     //         ImGui::MenuItem("Show Rendered Triangles", nullptr, true);
//     //         ImGui::MenuItem("Toggle VSync", nullptr, true);
//     //         ImGui::MenuItem("Toggle Wireframe", nullptr, true);

//     //         ImGui::Checkbox("Wireframe Mode", &m_WireframeMode);
//     //         ImGui::EndMenu();
//     //     }
//     //     ImGui::EndMenuBar();
//     // }

//     ImVec2 wsize{ ImGui::GetContentRegionAvail() };
//     ImGui::Image(reinterpret_cast<ImTextureID>(m_Framebuffer->GetColorbufferID()), wsize, ImVec2(0, 1), ImVec2(1, 0));
//     ImGui::SetCursorPos(ImVec2(5, 25)); ImGui::Text("Viewport Size: (%f, %f)", wsize.x, wsize.y);
//     ImGui::SetCursorPos(ImVec2(5, 45)); ImGui::Text("FPS ImGui: %f", 1.0f / io.DeltaTime);
//     ImGui::SetCursorPos(ImVec2(5, 65)); ImGui::Text("FPS Viewport: %f", 1.0f / timestamp.DeltaTime);
//     ImGui::SetCursorPos(ImVec2(5, 85)); ImGui::Text("Triangles rendered: %d", m_RendererContext->GetPrimitivesRendered());
//     ImGui::SetCursorPos(ImVec2(5, 105));
//     if (ImGui::Button("Toggle VSync"))
//     {
//         m_Window->SetVSync(!m_Window->IsVSync());
//     }
//     ImGui::SetCursorPos(ImVec2(5, 125));
//     if (ImGui::Button("Toggle Wireframe"))
//     {
//         glPolygonMode(GL_FRONT_AND_BACK, m_WireframeMode ? GL_LINE : GL_FILL);
//         m_WireframeMode = !m_WireframeMode;
}

//     // update-viewport-size-and-position-begin
//     // {
//     //     const auto& [minx, miny]{ ImGui::GetWindowContentRegionMin() };
//     //     const auto& [maxx, maxy]{ ImGui::GetWindowContentRegionMax() };

//     //     const auto& windowPosition{ m_Window->GetPosition() };
//     //     const auto& viewportPosition{ ImGui::GetWindowPos() };

//     //     m_ViewportPosition = {
//     //         (minx + viewportPosition.x) - windowPosition.x,
//     //         (miny + viewportPosition.y) - windowPosition.y,
//     //     };

//     //     m_ViewportSize = { maxx - minx, maxy - miny, };
//     // }
//     // update-viewport-size-and-position-end

//     ImGui::End();
//     ImGui::PopStyleVar();
// }

void Application::PanelShader(ImGuiIO& io, const Timestamp& timestamp)
{
//     ImGui::Begin("Shader Preview");

//     const auto& shaderInfo{ m_ShaderData.ShaderDataVector };
//     static std::size_t contentIndex{ 0u };
//     static bool openPopup{ false };

//     static ImGuiTableFlags tableFlags = ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableRowFlags_Headers;
//     if (ImGui::BeginTable("Program Shader Sources", 3, tableFlags))
//     {
//         ImGui::TableSetupColumn("Shader Type");
//         ImGui::TableSetupColumn("Source Filepath");
//         ImGui::TableSetupColumn("Preview");
//         ImGui::TableHeadersRow();

//         for (int i{ 0 }; i < shaderInfo.size(); ++i)
//         {
//             ImGui::TableNextRow();
//             ImGui::TableSetColumnIndex(0);
//             ImGui::Text("%s", shaderInfo[i].Type.data());
//             ImGui::TableSetColumnIndex(1);
//             ImGui::Text("%s", shaderInfo[i].Source.GetPath().data());
//             ImGui::TableSetColumnIndex(2);
//             if (ImGui::Button(std::string{ "Show source code!##" + std::to_string(i) }.data()))
//             {
//                 ImGui::OpenPopup("Source Code");
//                 contentIndex = i;
//                 openPopup = true;
//             }
//         }

//         if (ImGui::BeginPopupModal("Source Code", &openPopup))
//         {
//             ImGui::Text("%s", shaderInfo[contentIndex].Source.GetContent().data());
//             ImGui::EndPopup();
//         }

//         ImGui::EndTable();
//     }

//     ImGui::End();
}

std::unique_ptr<Application> CreateApplication() noexcept
{
    ApplicationProps props{};
    props.Name       = "UserApplication";
    props.WindowSize = { 800u, 600u, };

    return std::make_unique<Application>(props);
}
