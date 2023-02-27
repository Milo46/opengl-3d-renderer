#include "UserScene.hpp"

#include <Renderer/Loaders/OBJLoader.hpp>

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
