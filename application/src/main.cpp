#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <spdlog/spdlog.h>
#include <filesystem>

#include "ImGui/ImGuiBuild.hpp"
#include "Renderer/Shader.hpp"

static void GLFWErrorCallback(int error, const char* desc)
{
    spdlog::error("GLFW Error {}: {}", error, desc);
}

int main(int argc, char** argv)
{
    const auto rootFilepath{ std::filesystem::path{ __FILE__ }.append("../../") };
    std::filesystem::current_path(rootFilepath);

    glfwSetErrorCallback(GLFWErrorCallback);
    if (!glfwInit()) return EXIT_FAILURE;

    auto window{ glfwCreateWindow(800u, 600u, "Dear ImGui OpenGL", nullptr, nullptr) };
    if (!window) return EXIT_FAILURE;

    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        spdlog::critical("Failed to initialize OpenGL context!");
        return false;
    }
    glfwSwapInterval(1);

    // Allow debug logs to be visible.
    spdlog::set_level(spdlog::level::debug);
    
    auto customShader{ Renderer::Shader::Create({
        .Sources = {
            { Renderer::ShaderType::Vertex,   { "assets/shaders/vertex.glsl",   }, },
            { Renderer::ShaderType::Fragment, { "assets/shaders/fragment.glsl", }, },
        },
    }) };
    spdlog::info("Custom shader compilation result: {}", customShader->Compile());
    spdlog::info("Custom shader linking result: {}", customShader->Link());

    const auto customShaderData{ Renderer::ShaderDataExtractor{ customShader } };

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io{ ImGui::GetIO() };
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGui::StyleColorsDark();
    ImGuiStyle& style{ ImGui::GetStyle() };
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init((char*)glGetString(460));

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glfwSwapBuffers(window);

        glClearColor(0.2f, 0.4f, 0.6f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
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

        const auto& shaderInfo{ customShaderData.ShaderData };

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

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            const auto contextBackup{ glfwGetCurrentContext() };
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(contextBackup);
        }
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return EXIT_SUCCESS;
}
