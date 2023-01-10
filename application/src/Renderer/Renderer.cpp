#include "Renderer.hpp"

#include <spdlog/spdlog.h>

#include "Renderer/VertexArray.hpp"
#include "Renderer/Texture2D.hpp"
#include "Renderer/Shader.hpp"

#include "Extensions/Renderer/BuffersVectorProps.hpp"

RENDERER_CODE_BEGIN

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

struct RendererVertex
{
    glm::vec3 Position;
    glm::vec2 Texcoord;
};

struct RendererData
{
    std::shared_ptr<VertexArray> RectangleVA;
    std::shared_ptr<Texture2D> FlatTexture;
    std::shared_ptr<Shader> FlatShader;

    std::size_t TrianglesCount{ 0u };
    std::size_t TrianglesCountTemp{ 0u };
};

static RendererData* s_RendererData{ nullptr };

const std::shared_ptr<Shader>& Renderer2D::GetFlatShader() noexcept
{
    return s_RendererData->FlatShader;
}

std::size_t Renderer2D::GetTriangleCount() noexcept
{
    return s_RendererData->TrianglesCount;
}

bool Renderer2D::Initialize()
{
    if (s_RendererData)
    {
        spdlog::critical("[Renderer2D] Failed to initialize Renderer2D!");
        return false;
    }

    s_RendererData = new RendererData{};

    // It should probably stay how it is, maybe the time will come...
    const float defaultZCoordinate{ 0.0f };

    const std::array<RendererVertex, 4u> rectangleVertices = { {
        // position                               texcoord
        { {  0.5f,  0.5f, defaultZCoordinate, }, { 1.0f, 1.0f, }, }, // right top
        { {  0.5f, -0.5f, defaultZCoordinate, }, { 1.0f, 0.0f, }, }, // right bottom
        { { -0.5f, -0.5f, defaultZCoordinate, }, { 0.0f, 0.0f, }, }, // left  bottom
        { { -0.5f,  0.5f, defaultZCoordinate, }, { 0.0f, 1.0f, }, }, // left  top
    } };

    const std::array<unsigned int, 6u> rectangleIndices = { {
        0u, 1u, 3u, // first  triangle
        1u, 2u, 3u, // second triangle
    } };

    // Represent indices as an array of triangles, not individual vertices.
    // const std::array<glm::uvec3, 2u> rectangleIndices = { {
    //     { 0u, 1u, 3u, }, // first  triangle
    //     { 1u, 2u, 3u, }, // second triangle
    // } };

    auto vertexBuffer{ Create<VertexBuffer>({
        .Data   = rectangleVertices.data(),
        .Size   = rectangleVertices.size() * sizeof(decltype(rectangleVertices)::value_type),
        .Layout = {
            { LayoutDataType::Float3, "a_Position", },
            { LayoutDataType::Float2, "a_Texcoord", },
        },
    }) };

    auto indexBuffer{ Create<IndexBuffer>({
        .Data  = rectangleIndices.data(),
        .Count = rectangleIndices.size(),
    }) };

    s_RendererData->RectangleVA = Create<VertexArray>({
        .VertexBuffer = vertexBuffer,
        .IndexBuffer  = indexBuffer,
    });

    s_RendererData->FlatTexture = Create<Texture2D>({});

    s_RendererData->FlatShader = Create<Shader>({
        .Sources = {
            { ShaderType::Vertex,   { "assets/shaders/vertex.glsl",   }, },
            { ShaderType::Fragment, { "assets/shaders/fragment.glsl", }, },
        },
    });
    if (!s_RendererData->FlatShader->Compile()) return false;
    if (!s_RendererData->FlatShader->Link())    return false;

    return true;
}

void Renderer2D::Shutdown()
{
    delete s_RendererData;
}

void Renderer2D::BeginScene(const OrthographicCamera& camera)
{
    s_RendererData->TrianglesCountTemp = 0u;

    s_RendererData->FlatShader->Bind();
    s_RendererData->FlatShader->SetUniform("u_ViewMatrix", camera.GetViewMatrix());
    s_RendererData->FlatShader->SetUniform("u_ProjectionMatrix", camera.GetProjectionMatrix());
}

void Renderer2D::BeginScene(const Camera* camera)
{
    s_RendererData->TrianglesCountTemp = 0u;

    s_RendererData->FlatShader->Bind();
    s_RendererData->FlatShader->SetUniform("u_ViewMatrix", camera->GetViewMatrix());
    s_RendererData->FlatShader->SetUniform("u_ProjectionMatrix", camera->GetProjectionMatrix());
}

void Renderer2D::EndScene()
{
    s_RendererData->TrianglesCount = s_RendererData->TrianglesCountTemp;
}

void Renderer2D::DrawRectangle(const glm::vec3& size, const glm::vec3& position, const glm::vec3& color)
{
    const auto model{ ComposeModelMatrix(position, size) };

    s_RendererData->FlatShader->SetUniform("u_ModelMatrix", model);
    s_RendererData->FlatShader->SetUniform("u_Color", color);

    s_RendererData->RectangleVA->Bind();
    RenderCommand::DrawIndexed(s_RendererData->RectangleVA);

    s_RendererData->TrianglesCountTemp +=
        s_RendererData->RectangleVA->GetIndexBuffer()->GetCount() / 3u;
}

RENDERER_CODE_END
