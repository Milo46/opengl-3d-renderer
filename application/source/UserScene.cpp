#include "UserScene.hpp"

#include <Crenderr/Renderer/Loaders/OBJLoader.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

#include <Crenderr/ImGui/ImGuiContext.hpp>

UserScene::UserScene(std::unique_ptr<Window>& windowRef)
    : Scene{ windowRef },
      m_RendererContext{ std::make_unique<Renderer::Renderer3DInstance>() },
      m_Camera{ {
        .Position = { 0.0f, 0.0f, 2.0f, },
        .Ratio = Scene::GetWindow()->GetAspectRatio(),
      } } {}

bool UserScene::OnInit()
{
    if (!m_RendererContext->OnInitialization()) return false;

    m_Model = LoadOBJModel("assets/models/spaceship.obj", FaceType::Triangle);
    if (!m_Model->OnInitialize()) return false;

    m_DiffuseMap = Renderer::AllocateResource<Renderer::Texture2D>({
        .Filepath = "assets/textures/spaceship/diffuse_map.jpg",
    });
    if (!m_DiffuseMap->OnInitialize()) return false;
    m_DiffuseMap->Unbind();

    m_SpecularMap = Renderer::AllocateResource<Renderer::Texture2D>({
        .Filepath = "assets/textures/spaceship/specular_map.jpg",
    });
    if (!m_SpecularMap->OnInitialize()) return false;
    m_SpecularMap->Unbind();

    m_EmissionMap = Renderer::AllocateResource<Renderer::Texture2D>({
        .Filepath = "assets/textures/spaceship/emissive_map.jpg",
    });
    if (!m_EmissionMap->OnInitialize()) return false;
    m_EmissionMap->Unbind();

    return true;
}

void UserScene::OnUpdate(const Timestamp& timestamp)
{
    { //glfw-code-begin

        // Prevents from imgui and mouse capture conflict.
        if (!ImGui::GetIO().WantCaptureMouse)
        {
            GLFWwindow* window{ Scene::GetWindow()->GetNativeWindow() };

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
    } //glfw-code-end

    if (m_IsMouseCaptured)
    {
        // spdlog::info("Cursor position: {} {}", m_CurrFrameCursorPos.x, m_CurrFrameCursorPos.y);

        const auto delta{ m_CurrFrameCursorPos - m_PrevFrameCursorPos };

        spdlog::info("Cursor speed: {} {}", delta.x, delta.y);

        double xpos{}, ypos{};
        glfwGetCursorPos(Scene::GetWindow()->GetNativeWindow(), &xpos, &ypos);

        m_PrevFrameCursorPos = m_CurrFrameCursorPos;
        m_CurrFrameCursorPos = { xpos, ypos };
    }

    m_Camera.OnUpdate(Scene::GetWindow()->GetAspectRatio());

    glm::vec3 newPosition{};
    newPosition.x = m_CameraArmLength * sin(timestamp.TotalTime);
    newPosition.z = m_CameraArmLength * cos(timestamp.TotalTime);

    m_Camera
        .SetPosition(newPosition)
        .SetLookDirection(glm::vec3(0.0f));
}

void UserScene::OnRender()
{
    m_RendererContext->BeginScene(&m_Camera);
    m_RendererContext->SetPointLight(m_Camera.GetPosition(), glm::vec3(1.0f));

    m_RendererContext->DrawArrays(m_Model, m_DiffuseMap, m_SpecularMap, m_EmissionMap);

    m_RendererContext->EndScene();
}

void UserScene::OnImGuiRender(ImGuiIO& io, const Timestamp& timestamp)
{
    ImGui::Begin("Scene parameters");
    ImGui::SliderFloat("Camera Arm Length", &m_CameraArmLength, 0.1f, 3.0f);
    ImGui::End();
}
