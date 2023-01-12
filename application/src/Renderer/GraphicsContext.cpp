#include "GraphicsContext.hpp"

#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#include <spdlog/spdlog.h>

#include <spdlog/spdlog.h>

RENDERER_CODE_BEGIN

GraphicsContext::GraphicsContext(GLFWwindow* window)
    : m_WindowHandle{ window } {}

bool GraphicsContext::Initialize()
{
    if (!m_WindowHandle)
    {
        // spdlog::critical("Window handle cannot be nullptr!");
        return false;
    }

    glfwMakeContextCurrent(m_WindowHandle);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        // spdlog::critical("[GLAD] Failed to initialize!");
        return false;
    }

    glEnable(GL_DEPTH_TEST);
    glfwSwapInterval(1);

    spdlog::info("OpenGL info:");
    spdlog::info("\tVendor: {}", glGetString(GL_VENDOR));
    spdlog::info("\tRenderer: {}", glGetString(GL_RENDERER));
    spdlog::info("\tVersion: {}", glGetString(GL_VERSION));

    return true;
}

void GraphicsContext::SwapBuffers()
{
    glfwSwapBuffers(m_WindowHandle);
}

bool GraphicsContext::SetVSync(const bool flag)
{
    glfwSwapInterval(flag);
    return flag;
}

RENDERER_CODE_END
