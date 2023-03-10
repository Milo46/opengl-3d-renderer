#include "Renderer.hpp"

#include "Renderer/Backend/VertexArray.hpp"
#include "Renderer/Backend/Texture2D.hpp"
#include "Renderer/Backend/Shader.hpp"

#include "Renderer/Loaders/OBJLoader.hpp"

#include <spdlog/spdlog.h>

NAMESPACE_BEGIN(Renderer)

const std::shared_ptr<Shader>& Renderer3DInstance::GetFlatShader() const noexcept
{
    return m_Storage->FlatShader;
}

std::size_t Renderer3DInstance::GetPrimitivesRendered() const noexcept
{
    return m_Storage->PrimitivesCount;
}

bool Renderer3DInstance::OnInitialization() noexcept
{
    if (m_Storage.get())
    {
        spdlog::warn("The current instance of a Renderer3D is already initialized!");
        return true;
    }

    m_Storage = std::make_unique<Renderer3DStorage>();

    const std::array<Vertex3D, 4u> rectangleVertices = { {
        // position                 normal                  texcoord
        { {  0.5f,  0.5f, 0.0f, }, { 0.0f, 0.0f, -1.0f, }, { 1.0f, 1.0f, }, }, // right top
        { {  0.5f, -0.5f, 0.0f, }, { 0.0f, 0.0f, -1.0f, }, { 1.0f, 0.0f, }, }, // right bottom
        { { -0.5f, -0.5f, 0.0f, }, { 0.0f, 0.0f, -1.0f, }, { 0.0f, 0.0f, }, }, // left  bottom
        { { -0.5f,  0.5f, 0.0f, }, { 0.0f, 0.0f, -1.0f, }, { 0.0f, 1.0f, }, }, // left  top
    } };

    // Represent indices as an array of triangles, not individual vertices.
    const std::array<glm::uvec3, 2u> rectangleIndices = { {
        { 0u, 1u, 3u, }, // first  triangle
        { 1u, 2u, 3u, }, // second triangle
    } };

    auto vertexBuffer{ AllocateResource<VertexBuffer>({
        .Data     = rectangleVertices.data(),
        .DataSize = rectangleVertices.size(),
        .VertSize = sizeof(decltype(rectangleVertices)::value_type),
        .Layout   = decltype(rectangleVertices)::value_type::c_Layout,
    }) };
    if (!vertexBuffer->OnInitialize()) return false;

    auto indexBuffer{ AllocateResource<IndexBuffer>({
        .Data  = rectangleIndices.data(),
        .Count = rectangleIndices.size() * (sizeof(decltype(rectangleIndices)::value_type) / sizeof(unsigned int)),
    }) };
    if (!indexBuffer->OnInitialize()) return false;

    m_Storage->PlaneVArray = AllocateResource<VertexArray>({
        .VertexBufferPtr = vertexBuffer,
        .IndexBufferPtr  = indexBuffer,
    });
    if (!m_Storage->PlaneVArray->OnInitialize()) return false;

    m_Storage->CubeVArray = LoadOBJModel("assets/models/cube.obj", FaceType::Quad);
    if (!m_Storage->CubeVArray->OnInitialize()) return false;

    m_Storage->FlatTexture = AllocateResource<Texture2D>({
        .Size = { 1u, 1u, },
    });
    if (!m_Storage->FlatTexture->OnInitialize()) return false;

    m_Storage->CubeTexture = AllocateResource<Texture2D>({
        .Filtering = TextureFiltering::Nearest,
        .Filepath  = "assets/textures/container.jpg",
    });
    if (!m_Storage->CubeTexture->OnInitialize()) return false;

    m_Storage->FlatShader = AllocateResource<Shader>({
        .Sources = {
            { ShaderType::Vertex,   { "assets/shaders/vertex.glsl",   }, },
            { ShaderType::Fragment, { "assets/shaders/fragment.glsl", }, },
        },
    });
    if (!m_Storage->FlatShader->Compile()) return false;
    if (!m_Storage->FlatShader->Link())    return false;

    return true;
}

void Renderer3DInstance::OnShutdown() noexcept
{
    m_Storage.~unique_ptr();
}

void Renderer3DInstance::BeginScene(Camera* camera) noexcept
{
    m_Storage->PrimitivesCountTemp = 0u;

    m_Storage->FlatShader->Bind();
    // m_Storage->FlatShader->SetUniform("u_ViewMatrix", camera->GetViewMatrix());
    // m_Storage->FlatShader->SetUniform("u_ProjectionMatrix", camera->GetProjectionMatrix());
    // m_Storage->FlatShader->SetUniform("u_ViewPosition", camera->GetPosition());

    m_Storage->FlatShader->SetUniform("u_ViewMatrix", camera->CalculateViewMatrix());
    m_Storage->FlatShader->SetUniform("u_ProjectionMatrix", camera->CalculateProjectionMatrix());
    m_Storage->FlatShader->SetUniform("u_ViewPosition", camera->Position);
}

void Renderer3DInstance::EndScene() noexcept
{
    m_Storage->PrimitivesCount = m_Storage->PrimitivesCountTemp;
}

void Renderer3DInstance::DrawPlane(const Translation& translation)
{
    m_Storage->FlatShader->SetUniform("u_ModelMatrix", translation.ComposeModelMatrix());

    m_Storage->FlatShader->SetUniform("u_Material.Ambient",  glm::vec3{ 0.1f, 0.1f, 0.1f, });
    m_Storage->FlatShader->SetUniform("u_Material.Diffuse",  glm::vec3{ 1.0f, 1.0f, 1.0f, });
    m_Storage->FlatShader->SetUniform("u_Material.Specular", glm::vec3{ 0.5f, 0.5f, 0.5f, });
    m_Storage->FlatShader->SetUniform("u_Material.Shininess", 32.0f);

    RenderCommand::DrawIndexed(m_Storage->PlaneVArray, m_Storage->CubeTexture);

    m_Storage->PrimitivesCountTemp +=
        m_Storage->PlaneVArray->GetIndexBuffer()->GetCount() / 3u;
}

void Renderer3DInstance::DrawCube(const Translation& translation, const glm::vec3& color)
{
    m_Storage->FlatShader->SetUniform("u_ModelMatrix", translation.ComposeModelMatrix());

    m_Storage->FlatShader->SetUniform("u_Material.Ambient",  glm::vec3{ 0.1f, 0.1f, 0.1f, });
    m_Storage->FlatShader->SetUniform("u_Material.Diffuse",  glm::vec3{ 1.0f, 1.0f, 1.0f, });
    m_Storage->FlatShader->SetUniform("u_Material.Specular", glm::vec3{ 0.5f, 0.5f, 0.5f, });
    m_Storage->FlatShader->SetUniform("u_Material.Shininess", 32.0f);

    RenderCommand::DrawArrays(m_Storage->CubeVArray);

    m_Storage->PrimitivesCountTemp +=
        m_Storage->CubeVArray->GetVertexBuffer()->GetSize() / 3u;
}

void Renderer3DInstance::SetPointLight(const glm::vec3& position, const glm::vec3& color)
{
    m_Storage->FlatShader->SetUniform("u_Light.Position", position);
    m_Storage->FlatShader->SetUniform("u_Light.Color", color);
}

void Renderer3DInstance::DrawArrays(
    const std::shared_ptr<VertexArray>& vertexArray,
    const std::shared_ptr<Texture2D>& diffuse,
    const std::shared_ptr<Texture2D>& specular,
    const std::shared_ptr<Texture2D>& emission,
    bool wireframe)
{
    Translation translation{ .Scale = glm::vec3(0.1f), };

    m_Storage->FlatShader->SetUniform("u_ModelMatrix", translation.ComposeModelMatrix());

    m_Storage->FlatShader->SetUniform("u_Material.Ambient",  glm::vec3{ 0.1f, 0.1f, 0.1f, });
    m_Storage->FlatShader->SetUniform("u_Material.Diffuse",  glm::vec3{ 1.0f, 1.0f, 1.0f, });
    m_Storage->FlatShader->SetUniform("u_Material.Specular", glm::vec3{ 0.5f, 0.5f, 0.5f, });
    m_Storage->FlatShader->SetUniform("u_Material.Shininess", 32.0f);

    if (diffuse.get())
    {
    }

    glActiveTexture(GL_TEXTURE0);
    diffuse->Bind();
    m_Storage->FlatShader->SetUniform<int>("u_DiffuseTexture", GL_TEXTURE0 - GL_TEXTURE0);

    glActiveTexture(GL_TEXTURE1);
    specular->Bind();
    m_Storage->FlatShader->SetUniform<int>("u_SpecularTexture", GL_TEXTURE1 - GL_TEXTURE0);

    glActiveTexture(GL_TEXTURE2);
    emission->Bind();
    m_Storage->FlatShader->SetUniform<int>("u_EmissionTexture", GL_TEXTURE2 - GL_TEXTURE0);

    RenderCommand::DrawArrays(vertexArray);

    if (wireframe)
    {
        glLineWidth(3.0f);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        
        m_Storage->FlatShader->SetUniform("u_Color", glm::vec3(0.0f));
        RenderCommand::DrawArrays(vertexArray);

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

NAMESPACE_END(Renderer)
