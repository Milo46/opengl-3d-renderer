#include "GraphicsContext.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

NAMESPACE_BEGIN(Renderer)

/**
 * Source: https://www.khronos.org/opengl/wiki/OpenGL_Error
 */
void GLAPIENTRY GLADErrorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
    fprintf(stderr, "GL CALLBACK %s type = 0x%x, severity = 0x%x, message = %s\n", (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""), type, severity, message);
}

GraphicsContext::GraphicsContext(GLFWwindow* window)
    : m_WindowHandle{ window } {}

bool GraphicsContext::Initialize() noexcept
{
    if (!m_WindowHandle)
    {
        spdlog::critical("Window handle cannot be nullptr!");
        return false;
    }

    glfwMakeContextCurrent(m_WindowHandle);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        spdlog::critical("[GLAD] Failed to initialize!");
        return false;
    }

    // glEnable(GL_DEBUG_OUTPUT);
    // glDebugMessageCallback(GLADErrorCallback, nullptr);

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

bool GraphicsContext::SetVSync(bool flag)
{
    glfwSwapInterval(flag);
    return flag;
}

NAMESPACE_END(Renderer)
