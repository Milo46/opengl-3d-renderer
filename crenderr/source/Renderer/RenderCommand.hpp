#pragma once

#include "Renderer/RendererCore.hpp"

#include "Renderer/Backend/VertexArray.hpp"
#include "Renderer/Backend/Texture2D.hpp"

#include <glm/glm.hpp>

NAMESPACE_BEGIN(Renderer)

namespace RenderCommand
{
    void SetViewport(int x, int y, int width, int height);

    void SetDepthTest(bool flag);
    
    void SetClearColor(const glm::vec4& color);
    void Clear();

    void DrawArrays(const std::shared_ptr<VertexArray>& vertexArray);

    void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray);
    void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, const std::shared_ptr<Texture2D>& texture);
}

NAMESPACE_END(Renderer)
