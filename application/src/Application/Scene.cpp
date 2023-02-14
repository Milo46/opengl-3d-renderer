#include "Scene.hpp"

#include <glad/glad.h>
#include <glfw/glfw3.h>

UserScene::UserScene(std::unique_ptr<Window>& windowRef)
    : Scene{ windowRef },
      m_RendererContext{ std::make_unique<Renderer::Renderer3DInstance>() },
      m_Camera{ {
        .Position = { 0.0f, 0.0f, 2.0f, },
        .Ratio = Scene::GetWindow()->GetAspectRatio(),
      } } {}

bool UserScene::OnInitialization()
{
    if (!m_RendererContext->OnInitialization()) return false;

    const auto modelData{ LoadOBJFile("assets/models/spaceship.obj", FaceType::Triangle) };
    std::vector<Renderer::Renderer3DInstance::Vertex> convertedModelData(modelData.Data.size());
    for (std::size_t i = 0u; i < modelData.Data.size(); ++i)
    {
        const auto& vertex{ modelData.Data[i] };
        convertedModelData[i] = { vertex.Position, vertex.Normal, vertex.Texcoord, };
    }

    auto modelVB{ Renderer::AllocateResource<Renderer::VertexBuffer>({
        .Data     = convertedModelData.data(),
        .DataSize = convertedModelData.size(),
        .VertSize = sizeof(decltype(convertedModelData)::value_type),
        .Layout   = decltype(convertedModelData)::value_type::c_Layout,
    }) };
    if (!modelVB->OnInitialize()) return false;

    m_Model = Renderer::AllocateResource<Renderer::VertexArray>({
        .VertexBufferPtr = modelVB,
    });
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

void UserScene::OnUpdate(const Timestamp& timestamp) noexcept
{
    m_Camera.OnUpdate(Scene::GetWindow()->GetAspectRatio());

    glm::vec3 newPosition{};
    newPosition.x = m_CameraArmLength * sin(timestamp.TotalTime);
    newPosition.z = m_CameraArmLength * cos(timestamp.TotalTime);

    m_Camera
        .SetPosition(newPosition)
        .SetLookDirection(glm::vec3(0.0f));

    // static bool holding{ false };

    // double xpos{}, ypos{};
    // glfwGetCursorPos(Scene::GetWindow()->GetNativeWindow(), &xpos, &ypos);
    // if (glfwGetMouseButton(Scene::GetWindow()->GetNativeWindow(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS &&
    //     holding == false)
    // {
    //     holding = true;
    //     spdlog::info("Holding the viewport!");

    //     glfwSetInputMode(Scene::GetWindow()->GetNativeWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // }

    // if (glfwGetMouseButton(Scene::GetWindow()->GetNativeWindow(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE &&
    //     holding == true)
    // {
    //     holding = false;
    //     spdlog::info("Released the viewport!");

    //     glfwSetInputMode(Scene::GetWindow()->GetNativeWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    // }

    // if (holding)
    // {
    // }
}

void UserScene::OnRender() noexcept
{
    m_RendererContext->BeginScene(&m_Camera);
    m_RendererContext->SetPointLight(m_Camera.GetPosition(), glm::vec3(1.0f));

    m_RendererContext->DrawArrays(m_Model, m_DiffuseMap, m_SpecularMap, m_EmissionMap, false);

    m_RendererContext->EndScene();
}

void UserScene::OnImGuiRender(ImGuiIO& io, const Timestamp& timestamp) noexcept
{
    ImGui::Begin("Scene parameters");
    ImGui::SliderFloat("Camera Arm Length", &m_CameraArmLength, 0.1f, 3.0f);
    ImGui::End();
}
