#pragma once

#include "Renderer/RendererCore.hpp"
#include "Renderer/VertexArray.hpp"
#include "Renderer/Texture2D.hpp"

#include <glm/glm.hpp>

RENDERER_CODE_BEGIN

namespace RenderCommand
{
    void SetViewport(int x, int y, int width, int height);
    
    void SetDepthTest(bool flag);

    void SetClearColor(const glm::vec4& color);
    void Clear();

    void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray);
    void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, const std::shared_ptr<Texture2D>& texture);

    // Experimental
    void DrawArrays(const std::shared_ptr<VertexArray>& vertexArray, std::size_t count);
}

RENDERER_CODE_END
