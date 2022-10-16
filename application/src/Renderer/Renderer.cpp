#include "Renderer.hpp"

#include <spdlog/spdlog.h>
#include <glm/gtc/matrix_transform.hpp>

#include "Renderer/VertexArray.hpp"
#include "Renderer/Shader.hpp"

#include "Extensions/Renderer/BuffersVectorProps.hpp"

RENDERER_CODE_BEGIN

OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
    : m_ProjectionMatrix{ glm::ortho(left, right, bottom, top, -1.0f, 1.0f) },
      m_ViewMatrix{ glm::identity<glm::mat4>() } {}

void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
{
    m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
}

OrthographicCamera& OrthographicCamera::SetPosition(const glm::vec3& position) noexcept
{
    m_Position = position;
    OrthographicCamera::RecalculateData();
    return *this;
}

OrthographicCamera& OrthographicCamera::SetRotation(const float rotation) noexcept
{
    m_Rotation = rotation;
    OrthographicCamera::RecalculateData();
    return *this;
}

void OrthographicCamera::RecalculateData() noexcept
{
    glm::mat4 transform{
        glm::translate(glm::identity<glm::mat4>(), m_Position) *
        glm::rotate(glm::identity<glm::mat4>(), glm::radians(m_Rotation), glm::vec3(0, 0, 1))
    };

    m_ViewMatrix = glm::inverse(transform);
}

static auto ComposeModelMatrix(const glm::vec3& translation, const glm::vec3& rotation, const glm::vec3& scale) noexcept
{
    auto model{ glm::identity<glm::mat4>() };
    model = glm::translate(model, translation);
    model = glm::rotate(model, glm::radians(rotation.x), { 1.0f, 0.0f, 0.0f, });
    model = glm::rotate(model, glm::radians(rotation.y), { 0.0f, 1.0f, 0.0f, });
    model = glm::rotate(model, glm::radians(rotation.z), { 0.0f, 0.0f, 1.0f, });
    model = glm::scale(model, scale);

    return model;
}

static auto ComposeModelMatrix(const glm::vec3& translation, const glm::vec3& scale) noexcept
{
    auto model{ glm::identity<glm::mat4>() };
    model = glm::translate(model, translation);
    model = glm::scale(model, scale);

    return model;
}

struct RendererData
{
    std::shared_ptr<VertexArray> RectangleVA;
    std::shared_ptr<Shader> FlatShader;

    std::size_t TrianglesCount{ 0u };
};

static RendererData* s_RendererData{ nullptr };

bool Renderer2D::Initialize()
{
    if (s_RendererData)
    {
        spdlog::critical("[Renderer2D] Failed to initialize Renderer2D!");
        return false;
    }

    s_RendererData = new RendererData{};

    std::vector<float> vertices{
         0.5f,  0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f,  
    };

    std::vector<unsigned int> indices{
        0u, 1u, 3u,
        1u, 2u, 3u,  
    };

    auto vertexBuffer{ Create<VertexBuffer, Extension::VertexBufferPropsVector>({
        .VectorData = vertices,
        .Layout = {
            { LayoutDataType::Float3, "a_Position", },
        },
    }) };

    auto indexBuffer{ Create<IndexBuffer, Extension::IndexBufferPropsVector>({
        .VectorData = indices,
    }) };

    s_RendererData->RectangleVA = Create<VertexArray>({
        .VertexBuffer = vertexBuffer,
        .IndexBuffer = indexBuffer,
    });

    s_RendererData->FlatShader = Create<Shader>({
        .Sources = {
            { ShaderType::Vertex,   { "assets/shaders/vertex.glsl",   }, },
            { ShaderType::Fragment, { "assets/shaders/fragment.glsl", }, },
        },
    });
    if (!s_RendererData->FlatShader->Compile()) return false;
    if (!s_RendererData->FlatShader->Link()) return false;

    return true;
}

void Renderer2D::Shutdown()
{
    delete s_RendererData;
}

void Renderer2D::BeginScene(const OrthographicCamera& camera)
{
    s_RendererData->FlatShader->Bind();
    s_RendererData->FlatShader->SetUniform("u_ViewMatrix", camera.GetViewMatrix());
    s_RendererData->FlatShader->SetUniform("u_ProjectionMatrix", camera.GetProjectionMatrix());
}

void Renderer2D::EndScene()
{
}

void Renderer2D::DrawRectangle(const glm::vec3& size, const glm::vec3& position, const glm::vec3& color)
{
    const auto model{ ComposeModelMatrix(position, size) };

    s_RendererData->FlatShader->SetUniform("u_Model", model);
    s_RendererData->FlatShader->SetUniform("u_Color", color);

    s_RendererData->RectangleVA->Bind();
    glDrawElements(GL_TRIANGLES, s_RendererData->RectangleVA->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
    // RenderCommand::DrawIndexed(s_RendererData->RectangleVA);
}

RENDERER_CODE_END
