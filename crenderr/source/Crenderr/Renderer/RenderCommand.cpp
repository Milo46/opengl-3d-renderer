#include "RenderCommand.hpp"

#include <glad/glad.h>

NAMESPACE_BEGIN(Renderer)
NAMESPACE_BEGIN(RenderCommand)

void SetViewport(int x, int y, int width, int height)
{
    glViewport(x, y, width, height);
}

void SetDepthTest(bool flag)
{
    return flag
        ? glEnable(GL_DEPTH_TEST)
        : glDisable(GL_DEPTH_TEST);
}

void SetClearColor(const glm::vec4& color)
{
    glClearColor(color.r, color.g, color.b, color.a);
}

void Clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void DrawArrays(const std::shared_ptr<VertexArray>& vertexArray)
{
    vertexArray->Bind();
    glDrawArrays(GL_TRIANGLES, 0, { static_cast<GLsizei>(vertexArray->GetVertexBuffer()->GetSize()) });
}

void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
{
    vertexArray->Bind();
    glDrawElements(GL_TRIANGLES, { static_cast<GLsizei>(vertexArray->GetIndexBuffer()->GetCount()) }, GL_UNSIGNED_INT, nullptr);
}

void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, const std::shared_ptr<Texture2D>& texture)
{
    glActiveTexture(GL_TEXTURE0);
    texture->Bind();

    RenderCommand::DrawIndexed(vertexArray);
}

NAMESPACE_END(RenderCommand)
NAMESPACE_END(Renderer)
