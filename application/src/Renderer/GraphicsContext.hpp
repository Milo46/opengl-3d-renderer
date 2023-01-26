#pragma once

#include "RendererCore.hpp"

struct GLFWwindow;

NAMESPACE_BEGIN(Renderer)

class GraphicsContext
{
public:
    explicit GraphicsContext(GLFWwindow* window);

    bool Initialize() noexcept;

public:
    void SwapBuffers();
    bool SetVSync(bool flag);

private:
    GLFWwindow* m_WindowHandle{};
};

NAMESPACE_END(Renderer)
