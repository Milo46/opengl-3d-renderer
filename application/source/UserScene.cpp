#include "UserScene.hpp"

#include <Crenderr/Renderer/Loaders/OBJLoader.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

#include <Crenderr/ImGui/ImGuiContext.hpp>
#include <imgui.h>

std::shared_ptr<Renderer::VertexArray> CreateGrid(std::size_t slices, std::size_t gap)
{
    std::vector<Renderer::Vertex3D> vertices{};
    std::vector<glm::uvec4> indices{};

    for (uint32_t i = 0u; i <= slices; ++i)
    {
        for (uint32_t j = 0u; j <= slices; ++j)
        {
            glm::vec3 vertexPosition{
                (static_cast<float>(j) / static_cast<float>(slices)) * gap,
                0.0f,
                (static_cast<float>(i) / static_cast<float>(slices)) * gap,
            };

            vertices.push_back(Renderer::Vertex3D{ vertexPosition, {}, {} });
        }
    }

    for (uint32_t i = 0u; i < slices; ++i)
    {
        for (uint32_t j = 0u; j < slices; ++j)
        {
            int row1 =  i      * (slices + 1u);
            int row2 = (i + 1) * (slices + 1u);

            indices.push_back({ row1 + j, row1 + j + 1u, row1 + j + 1u, row2 + j + 1, });
            indices.push_back({ row2 + j + 1u, row2 + j, row2 + j, row1 + j });
        }
    }

    auto vertexBuffer{ Renderer::AllocateResource<Renderer::VertexBuffer>({
        .Data     = vertices.data(),
        .DataSize = vertices.size(),
        .VertSize = sizeof(decltype(vertices)::value_type),
        .Layout   = decltype(vertices)::value_type::c_Layout,
    }) };

    auto indexBuffer{ Renderer::AllocateResource<Renderer::IndexBuffer>({
        .Data  = indices.data(),
        .Count = indices.size() * (sizeof(decltype(indices)::value_type) / sizeof(unsigned int)),
    }) };

    auto vertexArray{ Renderer::AllocateResource<Renderer::VertexArray>({
        .VertexBufferPtr = vertexBuffer,
        .IndexBufferPtr  = indexBuffer,
    }) };

    vertexBuffer->OnInitialize();
    indexBuffer->OnInitialize();
    vertexArray->OnInitialize();

    return vertexArray;
}

UserScene::UserScene(std::unique_ptr<Window>& windowRef)
    : Scene{ windowRef },
      m_RendererContext{ std::make_unique<Renderer::Renderer3DInstance>() },
      m_Camera{ std::make_shared<Renderer::PerspectiveProjection>() }
{
    m_Camera.Position = { 0.0f, 0.0f, 2.0f, };
    m_Camera.GetProjection<Renderer::PerspectiveProjection>()->Ratio = Scene::GetWindow()->GetAspectRatio();
    m_Camera.GetProjection<Renderer::PerspectiveProjection>()->Near = 0.0001f;
}

bool UserScene::OnInit()
{
    if (!m_RendererContext->OnInitialization()) return false;

    m_Grid = CreateGrid(10u, 10u);

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
        const auto delta{ m_CurrFrameCursorPos - m_PrevFrameCursorPos };

        {
            m_ControllerAngles.x += delta.x * m_MouseSensitivity;
            m_ControllerAngles.y += delta.y * m_MouseSensitivity;

            if (m_ControllerAngles.x < -360.0f) m_ControllerAngles.x += 360.0f;
            if (m_ControllerAngles.x >  360.0f) m_ControllerAngles.x -= 360.0f;

            m_ControllerAngles.y = std::clamp(m_ControllerAngles.y, -89.9f, 89.9f);
        }

        double xpos{}, ypos{};
        glfwGetCursorPos(Scene::GetWindow()->GetNativeWindow(), &xpos, &ypos);

        m_PrevFrameCursorPos = m_CurrFrameCursorPos;
        m_CurrFrameCursorPos = { xpos, ypos };
    }

    if (auto projection = m_Camera.GetProjection<Renderer::PerspectiveProjection>())
        projection->Ratio = Scene::GetWindow()->GetAspectRatio();

    glm::vec3 newPosition{
        m_CameraArmLength * cos(glm::radians(m_ControllerAngles.x)) * cos(glm::radians(m_ControllerAngles.y)),
        m_CameraArmLength * sin(glm::radians(m_ControllerAngles.y)),
        m_CameraArmLength * sin(glm::radians(m_ControllerAngles.x)) * cos(glm::radians(m_ControllerAngles.y)),
    };

    newPosition += m_CameraFocusPoint;

    m_Camera.Position = newPosition;
    m_Camera.Rotation.x = 2 * 90.0f + m_ControllerAngles.x;
    m_Camera.Rotation.y = -m_ControllerAngles.y;
}

void UserScene::OnRender()
{
    m_RendererContext->BeginScene(&m_Camera);
    m_RendererContext->SetPointLight(m_Camera.Position, glm::vec3(1.0f));

    m_Grid->Bind();
    glDrawElements(GL_LINES, m_Grid->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);

    m_RendererContext->DrawArrays(m_Model, m_DiffuseMap, m_SpecularMap, m_EmissionMap);

    // Renderer::Translation cubeTranslation{};
    // cubeTranslation.Scale = glm::vec3(0.005f);
    // cubeTranslation.Position = m_CameraFocusPoint;
    // m_RendererContext->DrawCube(cubeTranslation, glm::vec3(0.0f));

    m_RendererContext->EndScene();
}

void UserScene::OnImGuiRender(ImGuiIO& io, const Timestamp& timestamp)
{
    ImGui::Begin("Scene parameters");
    ImGui::Text("Scene FPS: %f", 1.0f / timestamp.DeltaTime);
    ImGui::SliderFloat("Camera Arm Length", &m_CameraArmLength, 0.1f, 3.0f);
    ImGui::SliderFloat3("Camera Focus Point", glm::value_ptr(m_CameraFocusPoint), -5.0f, 5.0f);
    ImGui::SliderFloat("Camera X Axis Angle", &m_ControllerAngles.x, -360.0f, 360.0f);
    ImGui::SliderFloat("Camera Y Axis Angle", &m_ControllerAngles.y, -89.9f, 89.9f);
    ImGui::SliderFloat("Camera Sensitivity", &m_MouseSensitivity, 0.0f, 0.5f);

    static const char* projections[]{ "Orthographic", "Perspective", };
    static int projectionIndex{ 1 };
    ImGui::Combo("Camera Projection", &projectionIndex, projections, IM_ARRAYSIZE(projections));

    ImGui::Text("Camera Coordinates (x: %f, y: %f, z: %f)", m_Camera.Position.x, m_Camera.Position.y, m_Camera.Position.z);
    ImGui::End();
}
