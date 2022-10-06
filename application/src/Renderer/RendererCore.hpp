#pragma once

#define RENDERER_CODE_BEGIN namespace Renderer {
#define RENDERER_CODE_END   }

namespace Renderer
{
    // Unfortunately I cannot include glad here ;(
    // So I have to believe that glad types are correct.
    using RendererID    = unsigned int; // GLuint
    using RendererEnum  = unsigned int; // GLenum
    using RendererSizei = int;          // GLsizei

    const RendererID c_EmptyID{ 0u };

    class RendererElement
    {
    public:
        explicit RendererElement(const RendererID id);
        const RendererID GetHandle() const noexcept;

    protected:
        RendererID m_RendererID{ c_EmptyID };
    };
}
