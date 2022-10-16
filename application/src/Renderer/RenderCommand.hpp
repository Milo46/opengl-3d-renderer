#pragma once

#include "Renderer/RendererCore.hpp"
#include "Renderer/VertexArray.hpp"

#include <glm/glm.hpp>

RENDERER_CODE_BEGIN

namespace RenderCommand
{
    void SetViewport(int x, int y, int width, int height);

    void SetClearColor(const glm::vec4& color);
    void Clear();

    void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray);
}

RENDERER_CODE_END
