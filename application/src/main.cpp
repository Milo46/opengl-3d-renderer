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

    GLuint FBO{ 0u };
    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    GLuint texture{ 0u };
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800u, 600u, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

    GLuint RBO{ 0u };
    glGenRenderbuffers(1, &RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800u, 600u);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        spdlog::critical("Framebuffer is not complete!");
    glBindFramebuffer(GL_FRAMEBUFFER, 0u);

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

        // glBindFramebuffer(GL_FRAMEBUFFER, FBO);
        // glEnable(GL_DEPTH_TEST);

        glClearColor(0.2f, 0.4f, 0.6f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

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

    glBindFramebuffer(GL_FRAMEBUFFER, 0u);
    glDeleteFramebuffers(1, &FBO);

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return EXIT_SUCCESS;
}
