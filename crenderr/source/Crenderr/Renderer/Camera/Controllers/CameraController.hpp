#pragma once

#include "Renderer/RendererCore.hpp"
#include "Renderer/Camera/Camera.hpp"

struct GLFWwindow;

NAMESPACE_BEGIN(Renderer)

class CameraOrbitController
{
public:
    Camera CameraHandle{};

    glm::vec2 Angles{ glm::vec3(0.0f) };
    glm::vec3 FocusCenter{ glm::vec3(0.0f) };
    float FocusRadius{ 1.0f };
    float Sensitivity{ 0.07f };

public:
    CameraOrbitController() = default;

public:
    // Raw opengl code, has to stay until the Input component of the library is not fully developed!
    void OnUpdate(GLFWwindow* window);

private:
    bool m_IsMouseCaptured{ false };
    bool m_IsFirstCaptureFrame{ false };
    glm::vec2 m_PrevFrameCursorPos{};
    glm::vec2 m_CurrFrameCursorPos{};
};

NAMESPACE_END(Renderer)
