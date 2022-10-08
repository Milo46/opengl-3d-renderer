#include "Window.hpp"

#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

static void GLFWErrorCallback(int error, const char* desc) noexcept
{
    spdlog::error("GLFW Error {}: {}", error, desc);
}

static void WindowFramebufferCallback(GLFWwindow* window, int width, int height)
{
    auto userPointer{ static_cast<Window*>(glfwGetWindowUserPointer(window)) };

    userPointer->m_Size.x = width;
    userPointer->m_Size.y = height;

    glViewport(0, 0, width, height);
}

static void WindowKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    auto userPointer{ static_cast<Window*>(glfwGetWindowUserPointer(window)) };

    if (key == GLFW_KEY_F11 && action == GLFW_PRESS)
        userPointer->SetFullscreen(!userPointer->m_IsFullscreen);
}

static void WindowFocusCallback(GLFWwindow* window, int focused)
{
    auto userPointer{ static_cast<Window*>(glfwGetWindowUserPointer(window)) };
    userPointer->m_IsFocused = static_cast<bool>(focused);
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
    if (!glfwInit())
    {
        spdlog::critical("[GLFW] Failed to initialize!");
        return false;
    }

    m_Window = glfwCreateWindow({ m_Size.x }, { m_Size.y }, m_Title.c_str(), nullptr, nullptr);

    m_OpenGLContext.reset(new Renderer::GraphicsContext{ m_Window });
    if (!m_OpenGLContext->Initialize())
    {
        spdlog::critical("[GraphicsContext] Failed to initialize!");
        return false;
    }

    m_Monitor = glfwGetPrimaryMonitor();
    glfwGetWindowSize(m_Window, &m_Size.x, &m_Size.y);
    glfwGetWindowPos(m_Window, &m_Position.x, &m_Position.y);

    glfwSetWindowUserPointer(m_Window, this);
    glfwSetFramebufferSizeCallback(m_Window, WindowFramebufferCallback);
    glfwSetKeyCallback(m_Window, WindowKeyCallback);
    glfwSetWindowFocusCallback(m_Window, WindowFocusCallback);

    return true;
}

bool Window::IsOpen() noexcept
{
    return !glfwWindowShouldClose(m_Window);
}

void Window::OnUpdate() noexcept
{
    glfwPollEvents();
    m_OpenGLContext->SwapBuffers();
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
