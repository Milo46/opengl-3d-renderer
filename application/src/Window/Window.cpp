#include "Window.hpp"

#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

#include "Utility/Checker.hpp"

const Window::KeybindsContainer Window::c_DefaultKeybinds{
    { WindowAction::ToggleFullscreen, { GLFW_KEY_F11,    }, },
    { WindowAction::Close,            { GLFW_KEY_ESCAPE, }, },
};

static bool s_GLFWInitialized{ false };

static void GLFWErrorCallback(int error, const char* desc) noexcept
{
    spdlog::error("GLFW Error {}: {}", error, desc);
}

void WindowFramebufferCallback(GLFWwindow* window, int width, int height)
{
    auto userPointer{ static_cast<Window*>(glfwGetWindowUserPointer(window)) };

    userPointer->m_Size.x = width;
    userPointer->m_Size.y = height;

    glViewport(0, 0, width, height);
}

void WindowKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    auto userPointer{ static_cast<Window*>(glfwGetWindowUserPointer(window)) };

    for (const auto& [bindaction, binds] : userPointer->m_Keybinds)
    {
        for (const auto& bind : binds)
        {
            if (!(key == bind && action == GLFW_PRESS))
                continue;
            
            switch (bindaction)
            {
            case WindowAction::Close:
                userPointer->Close();
                break;
            
            case WindowAction::ToggleFullscreen:
                userPointer->SetFullscreen(!userPointer->m_IsFullscreen);
                break;

            case WindowAction::Miximize: userPointer->Maximize(); break;
            case WindowAction::Iconify:  userPointer->Iconify();  break;

            default: break;
            }
            break;
        }
    }
}

void WindowFocusCallback(GLFWwindow* window, int focused)
{
    auto userPointer{ static_cast<Window*>(glfwGetWindowUserPointer(window)) };
    userPointer->m_IsFocused = static_cast<bool>(focused);
}

void WindowPositionCallback(GLFWwindow* window, int xpos, int ypos)
{
    auto userPointer{ static_cast<Window*>(glfwGetWindowUserPointer(window)) };
    userPointer->m_Position = { xpos, ypos, };
}

Window::Window(const std::string_view title, const glm::ivec2& size) noexcept
    : m_Title{ title }, m_Size{ size } {}

Window::~Window()
{
    glfwDestroyWindow(m_Window);
    glfwTerminate();
}

bool Window::Initialize() noexcept
{
    glfwSetErrorCallback(GLFWErrorCallback);
    if (!Checker::PerformSequence(spdlog::level::critical, {
        { [&]() { return !s_GLFWInitialized; }, "[GLFW] Cannot create more than one window (for now)!", },
        { [&]() { return glfwInit();         }, "[GLFW] Failed to initialize!",                         },
    })) return false;

    s_GLFWInitialized = true;
    m_Window = glfwCreateWindow({ m_Size.x }, { m_Size.y }, m_Title.c_str(), nullptr, nullptr);

    m_GraphicsContext.reset(new Renderer::GraphicsContext{ m_Window });
    if (!m_GraphicsContext->Initialize())
    {
        spdlog::critical("[GraphicsContext] Failed to initialize!");
        return false;
    }
    m_GraphicsContext->SetVSync(m_IsVSync);

    m_Monitor = glfwGetPrimaryMonitor();
    glfwGetWindowSize(m_Window, &m_Size.x, &m_Size.y);
    glfwGetWindowPos(m_Window, &m_Position.x, &m_Position.y);

    glfwSetWindowUserPointer(m_Window, this);
    glfwSetFramebufferSizeCallback(m_Window, WindowFramebufferCallback);
    glfwSetKeyCallback(m_Window, WindowKeyCallback);
    glfwSetWindowFocusCallback(m_Window, WindowFocusCallback);
    glfwSetWindowPosCallback(m_Window, WindowPositionCallback);

    return true;
}

bool Window::IsOpen() noexcept
{
    return !glfwWindowShouldClose(m_Window);
}

void Window::OnUpdate() noexcept
{
    glfwPollEvents();
    m_GraphicsContext->SwapBuffers();
}

void Window::Maximize() noexcept
{
    glfwMaximizeWindow(m_Window);
}

void Window::Iconify() noexcept
{
    glfwIconifyWindow(m_Window);
}

void Window::Close() noexcept
{
    glfwSetWindowShouldClose(m_Window, 1);
}

void Window::SetKeybinds(const KeybindsContainer& keybinds) noexcept
{
    m_Keybinds = keybinds;
}

void Window::AddKeybinds(const KeybindsContainer& keybinds) noexcept
{
    for (const auto& [action, binds] : keybinds)
        for (const auto& bind : binds) m_Keybinds[action].insert(bind);
}

bool Window::IsFullscreen() const
{
    return m_IsFullscreen;
}

void Window::SetFullscreen(bool flag)
{
    if (m_IsFullscreen == flag) return;

    if (flag)
    {
        glfwGetWindowPos(m_Window, &m_Position.x, &m_Position.y);
        glfwGetWindowSize(m_Window, &m_SizeBackup.x, &m_SizeBackup.y);

        const GLFWvidmode* mode{ glfwGetVideoMode(m_Monitor) };
        glfwSetWindowSize(m_Window, mode->width, mode->height);
        glfwSetWindowMonitor(m_Window, m_Monitor, 0, 0, mode->width, mode->height, 0);
        m_IsFullscreen = true;
    }
    else
    {
        glfwSetWindowMonitor(m_Window, nullptr, m_Position.x, m_Position.y, m_SizeBackup.x, m_SizeBackup.y, 0);
        m_IsFullscreen = false;
    }
}

bool Window::IsFocused() const
{
    return m_IsFocused;
}

bool Window::IsVSync() const
{
    return m_IsVSync;
}

bool Window::SetVSync(const bool flag)
{
    return (m_IsVSync = m_GraphicsContext->SetVSync(flag));
}

const glm::ivec2& Window::GetSize() const
{
    return m_Size;
}

const glm::ivec2& Window::GetPosition() const
{
    return m_Position;
}

void Window::SetSize(const glm::ivec2& size)
{
    m_Size = size;
    glfwSetWindowSize(m_Window, m_Size.x, m_Size.y);
}

void Window::SetPosition(const glm::ivec2& position)
{
    m_Position = position;
    glfwSetWindowPos(m_Window, m_Position.x, m_Position.y);
}

float Window::GetAspectRatio() const
{
    return static_cast<float>(m_Size.x)
         / static_cast<float>(m_Size.y);
}
