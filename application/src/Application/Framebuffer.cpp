#include "Framebuffer.hpp"

#include <glad/glad.h>

RENDERER_CODE_BEGIN

Framebuffer::Framebuffer(const FramebufferProps& props) noexcept
    : m_Size{ props.Size } {}

Framebuffer::~Framebuffer()
{
    if (m_RendererID != c_EmptyValue<RendererID>)
        glDeleteFramebuffers(1, &m_RendererID);
}

bool Framebuffer::Initialize() noexcept
{
    if (m_RendererID != c_EmptyValue<RendererID>)
        glDeleteFramebuffers(1, &m_RendererID);
    
    glGenFramebuffers(1, &m_RendererID);
    glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

    m_TextureColorbuffer = Create<Texture2D>({ .Size = m_Size, });
    if (!m_TextureColorbuffer->Initialize()) return false;
    m_TextureColorbuffer->Bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TextureColorbuffer->GetHandle(), 0);

    glGenRenderbuffers(1, &m_Renderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, m_Renderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_TextureColorbuffer->GetWidth(), m_TextureColorbuffer->GetHeight());
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_Renderbuffer);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) return false;

    Framebuffer::Unbind();

    return true;
}

void Framebuffer::Bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, { m_RendererID });
}

void Framebuffer::Unbind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, { c_EmptyValue<RendererID> });
}

RENDERER_CODE_END
