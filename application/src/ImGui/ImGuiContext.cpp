#include "ImGuiContext.hpp"

#include "ImGuiBuild.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

bool ImGuiBuildContext::Initialize(const std::unique_ptr<Window>& window) noexcept
{
    IMGUI_CHECKVERSION();
    m_Context = ImGui::CreateContext();
    m_IO = &ImGui::GetIO();
    m_IO->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    m_IO->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    m_IO->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    // I'm not sure if it is supposed to be here :/
    m_IO->ConfigWindowsMoveFromTitleBarOnly = true;

    ImGui::StyleColorsDark();
    m_Style = &ImGui::GetStyle();
    if (m_IO->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        m_Style->WindowRounding = 0.0f;
        m_Style->Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    if (!ImGui_ImplGlfw_InitForOpenGL(window->GetNativeWindow(), true)) return false;
    if (!ImGui_ImplOpenGL3_Init((char*)glGetString(460))) return false;

    return true;
}

void ImGuiBuildContext::PreRender() noexcept
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void ImGuiBuildContext::PostRender() noexcept
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if (m_IO->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        const auto contextBackup{ glfwGetCurrentContext() };
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(contextBackup);
    }
}

void ImGuiBuildContext::Shutdown() noexcept
{
    if (!m_Context) return;

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
