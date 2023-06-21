#include "CameraController.hpp"

#include "ImGui/ImGuiContext.hpp"

#include <GLFW/glfw3.h>

NAMESPACE_BEGIN(Renderer)

void CameraOrbitController::OnUpdate(GLFWwindow* window)
{
    if (!ImGui::GetIO().WantCaptureMouse)
    {
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && !m_IsFirstCaptureFrame)
        {
            m_IsFirstCaptureFrame = true;

            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            m_IsMouseCaptured = true;

            double xpos{}, ypos{};
            glfwGetCursorPos(window, &xpos, &ypos);
            m_PrevFrameCursorPos = { xpos, ypos, };
            m_CurrFrameCursorPos = { xpos, ypos, };
        }

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            m_IsMouseCaptured = false;
            m_IsFirstCaptureFrame = false;
        }
    }

    if (m_IsMouseCaptured)
    {
        const auto delta{ m_CurrFrameCursorPos - m_PrevFrameCursorPos };

        Angles.x += delta.x * Sensitivity;
        if (Angles.x < -360.0f) Angles.x += 360.0f;
        if (Angles.x >  360.0f) Angles.x -= 360.0f;

        Angles.y = std::clamp(Angles.y + (delta.y * Sensitivity), -89.9f, 89.9f);

        double xpos{}, ypos{};
        glfwGetCursorPos(window, &xpos, &ypos);

        m_PrevFrameCursorPos = m_CurrFrameCursorPos;
        m_CurrFrameCursorPos = { xpos, ypos, };
    }

    glm::vec3 newPosition{
        FocusRadius * cos(glm::radians(Angles.x)) * cos(glm::radians(Angles.y)),
        FocusRadius * sin(glm::radians(Angles.y)),
        FocusRadius * sin(glm::radians(Angles.x)) * cos(glm::radians(Angles.y)),
    };

    CameraHandle.Position = newPosition + FocusCenter;
    CameraHandle.Rotation.x = 2 * 90.0f + Angles.x;
    CameraHandle.Rotation.y = -Angles.y;
}

NAMESPACE_END(Renderer)
