#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <spdlog/spdlog.h>

#include "ImGui/ImGuiBuild.hpp"

static void GLFWErrorCallback(int error, const char* desc)
{
    spdlog::error("GLFW Error {}: {}", error, desc);
}

int main(int argc, char** argv)
{
    glfwSetErrorCallback(GLFWErrorCallback);
    if (!glfwInit()) return EXIT_FAILURE;

    auto window{ glfwCreateWindow(800u, 600u, "Dear ImGui OpenGL", nullptr, nullptr) };
    if (!window) return EXIT_FAILURE;

    spdlog::info("{}", (bool)window);

    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        spdlog::critical("Failed to initialize OpenGL context!");
        return false;
    }
    glfwSwapInterval(1);

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

        ImGui::Begin("Scene");

        ImVec2 viewportPanelSize{ ImGui::GetContentRegionAvail() };
        ImGui::Image(0u, viewportPanelSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

        ImGui::End();

        ImGui::ShowDemoWindow();

        // static bool active{ true };
        // if (active)
        // {
        // ImGui::Begin("Hello Window", &active, ImGuiWindowFlags_MenuBar);
        // if (ImGui::BeginMenuBar())
        // {
        //     if (ImGui::BeginMenu("File"))
        //     {
        //         if (ImGui::MenuItem("Open..", "Ctrl+O")) {}
        //         if (ImGui::MenuItem("Save", "Ctrl+S")) {}
        //         if (ImGui::MenuItem("Close", "Ctrl+W")) { active = false; }
        //         ImGui::EndMenu();
        //     }
        //     ImGui::EndMenuBar();
        // }
        // ImGui::End();
        // }

        // ImGui::Begin("Second Window");
        // ImGui::Text("Wow");
        // ImGui::End();

        ImGui::Render();

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            auto contextBackup{ glfwGetCurrentContext() };
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
