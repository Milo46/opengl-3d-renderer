#pragma once

#include "RendererCore.hpp"

struct GLFWwindow;

RENDERER_CODE_BEGIN

class GraphicsContext
{
public:
    explicit GraphicsContext(GLFWwindow* window);

    bool Initialize();
    void SwapBuffers();

private:
    GLFWwindow* m_WindowHandle;
};

RENDERER_CODE_END
