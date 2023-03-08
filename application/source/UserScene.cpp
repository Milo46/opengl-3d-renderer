#include "UserScene.hpp"

#include <Crenderr/Renderer/Loaders/OBJLoader.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

#include <Crenderr/ImGui/ImGuiContext.hpp>

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
      m_Camera{ {
        .Position = { 0.0f, 0.0f, 2.0f, },
        .Ratio = Scene::GetWindow()->GetAspectRatio(),
      } } {}

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
            m_XAngle += delta.x * m_MouseSensitivity;
            m_YAngle += delta.y * m_MouseSensitivity;

            if (m_XAngle < -360.0f) m_XAngle += 360.0f;
            if (m_XAngle >  360.0f) m_XAngle -= 360.0f;

            m_YAngle = std::clamp(m_YAngle, -89.9f, 89.9f);
        }

        double xpos{}, ypos{};
        glfwGetCursorPos(Scene::GetWindow()->GetNativeWindow(), &xpos, &ypos);

        m_PrevFrameCursorPos = m_CurrFrameCursorPos;
        m_CurrFrameCursorPos = { xpos, ypos };
    }

    m_Camera.OnUpdate(Scene::GetWindow()->GetAspectRatio());

    glm::vec3 newPosition{
        m_CameraArmLength * cos(glm::radians(m_XAngle)) * cos(glm::radians(m_YAngle)),
        m_CameraArmLength * sin(glm::radians(m_YAngle)),
        m_CameraArmLength * sin(glm::radians(m_XAngle)) * cos(glm::radians(m_YAngle)),
    };

    m_Camera
        .SetPosition(newPosition)
        .SetLookDirection(glm::vec3(0.0f));
}

void UserScene::OnRender()
{
    m_RendererContext->BeginScene(&m_Camera);
    m_RendererContext->SetPointLight(m_Camera.GetPosition(), glm::vec3(1.0f));

    m_Grid->Bind();
    glDrawElements(GL_LINES, m_Grid->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);

    m_RendererContext->DrawArrays(m_Model, m_DiffuseMap, m_SpecularMap, m_EmissionMap);

    m_RendererContext->EndScene();
}

void UserScene::OnImGuiRender(ImGuiIO& io, const Timestamp& timestamp)
{
    ImGui::Begin("Scene parameters");
    ImGui::SliderFloat("Camera Arm Length", &m_CameraArmLength, 0.1f, 3.0f);
    ImGui::SliderFloat("Camera X Axis Angle", &m_XAngle, -360.0f, 360.0f);
    ImGui::SliderFloat("Camera Y Axis Angle", &m_YAngle, -89.9f, 89.9f);
    ImGui::SliderFloat("Camera Sensitivity", &m_MouseSensitivity, 0.0f, 0.5f);
    ImGui::Text("Camera Coordinates (x: %f, y: %f)", m_Camera.GetPosition().x, m_Camera.GetPosition().y);
    ImGui::End();
}
