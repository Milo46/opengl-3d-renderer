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
      m_RendererContext{ std::make_unique<Renderer::Renderer3DInstance>() }
{
    m_OrbitController.FocusCenter = { 0.0f, 0.0f, 0.0f, };
    m_OrbitController.FocusRadius = 2.0f;
    m_OrbitController.Angles.x = 90.0f;

    m_Camera.ProjectionHandle = std::make_shared<Renderer::PerspectiveProjection>();
    m_Camera.Position = { 0.0f, 0.0f, 2.0f, };
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

    m_SpecularMap = Renderer::AllocateResource<Renderer::Texture2D>({
        .Filepath = "assets/textures/spaceship/specular_map.jpg",
    });
    if (!m_SpecularMap->OnInitialize()) return false;

    m_EmissionMap = Renderer::AllocateResource<Renderer::Texture2D>({
        .Filepath = "assets/textures/spaceship/emissive_map.jpg",
    });
    if (!m_EmissionMap->OnInitialize()) return false;

    return true;
}

void UserScene::OnUpdate(const Timestamp& timestamp)
{
    // Link the aspect ratio updating with the camera event system!
    if (auto projection = m_Camera.GetProjection<Renderer::PerspectiveProjection>())
        projection->Ratio = Scene::GetWindow()->GetAspectRatio();

    GLFWwindow* window{ Scene::GetWindow()->GetNativeWindow() };
    m_OrbitController.OnUpdate(window);
}

void UserScene::OnRender()
{
    (m_RenderDoubleViewport
        ? UserScene::RenderDoubleViewport()
        : UserScene::RenderSingleViewport());
}

void UserScene::RenderSingleViewport()
{
    const auto& windowSize{ Scene::GetWindow()->GetSize() };
    Renderer::RenderCommand::SetViewport(0, 0, windowSize.x, windowSize.y);
    UserScene::RenderViewport();
}

void UserScene::RenderDoubleViewport()
{
    const auto& windowSize{ Scene::GetWindow()->GetSize() };

    m_Camera.ProjectionHandle = std::make_shared<Renderer::PerspectiveProjection>();
    Renderer::RenderCommand::SetViewport(0, 0, windowSize.x / 2, windowSize.y);
    UserScene::RenderViewport();

    m_Camera.ProjectionHandle = std::make_shared<Renderer::OrthographicProjection>();
    Renderer::RenderCommand::SetViewport(windowSize.x / 2, 0, windowSize.x / 2, windowSize.y);
    UserScene::RenderViewport();
}

void UserScene::RenderViewport()
{
    m_RendererContext->BeginScene(&m_Camera);
    m_RendererContext->SetPointLight(m_Camera.Position, glm::vec3(1.0f));

    m_Grid->Bind();
    glDrawElements(GL_LINES, m_Grid->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);

    m_RendererContext->DrawArrays(m_Model, m_DiffuseMap, m_SpecularMap, m_EmissionMap);

    m_RendererContext->EndScene();
}

void UserScene::OnImGuiRender(ImGuiIO& io, const Timestamp& timestamp)
{
    ImGui::Begin("Scene parameters");
    ImGui::Text("Scene FPS: %f", 1.0f / timestamp.DeltaTime);
    ImGui::SliderFloat("Camera Arm Length", &m_OrbitController.FocusRadius, 0.1f, 3.0f);
    ImGui::SliderFloat3("Camera Focus Point", glm::value_ptr(m_OrbitController.FocusCenter), -5.0f, 5.0f);
    ImGui::SliderFloat("Camera X Axis Angle", &m_OrbitController.Angles.x, -360.0f, 360.0f);
    ImGui::SliderFloat("Camera Y Axis Angle", &m_OrbitController.Angles.y, -89.9f, 89.9f);
    ImGui::SliderFloat("Camera Sensitivity", &m_OrbitController.Sensitivity, 0.0f, 0.5f);

    static const char* projections[]{ "Orthographic", "Perspective", };
    static int projectionIndex{ 1 };
    if (ImGui::Combo("Camera Projection", &projectionIndex, projections, IM_ARRAYSIZE(projections)))
    {
        if (projectionIndex == 0)
        {
            spdlog::info("Set Orthographic Projection!");
            m_Camera.ProjectionHandle = std::make_shared<Renderer::OrthographicProjection>();
        }
        else if (projectionIndex == 1)
        {
            spdlog::info("Set Perspective Projection!");
            m_Camera.ProjectionHandle = std::make_shared<Renderer::PerspectiveProjection>();
        }
    }

    if (auto projection = m_Camera.GetProjection<Renderer::PerspectiveProjection>())
    {
        ImGui::SliderFloat("Camera Fov", &projection->Fov, 1.0f, 45.0f);
    }
    if (auto projection = m_Camera.GetProjection<Renderer::OrthographicProjection>())
    {
        ImGui::SliderFloat("Camera Zoom", &projection->Zoom, 0.5f, 15.0f);
    }

    if (ImGui::Checkbox("Double Viewport", &m_RenderDoubleViewport))
    {
        spdlog::info("WHAhawhaw");
    }

    ImGui::Text("Camera Coordinates (x: %f, y: %f, z: %f)", m_Camera.Position.x, m_Camera.Position.y, m_Camera.Position.z);
    ImGui::End();
}
