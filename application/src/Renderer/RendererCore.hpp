#pragma once

#include <GLAD/glad.h>

#define RENDERER_CODE_BEGIN namespace Renderer {
#define RENDERER_CODE_END   }

namespace Renderer
{
    using RendererID   = GLuint;
    using RendererEnum = GLenum;
}
