#include "Framebuffer.hpp"

#include <glad/glad.h>

#include <spdlog/spdlog.h>

NAMESPACE_BEGIN(Renderer)

Framebuffer::Framebuffer(const FramebufferProps& props)
    : m_Size{ props.Size } {}

Framebuffer::~Framebuffer()
{
    if (m_Renderbuffer != c_EmptyValue<RendererID>)
        glDeleteRenderbuffers(1, &m_Renderbuffer);

    if (m_RendererID != c_EmptyValue<RendererID>)
        glDeleteFramebuffers(1, &m_RendererID);
}

bool Framebuffer::OnInitialize() noexcept
{
    if (m_RendererID != c_EmptyValue<RendererID>)
        glDeleteFramebuffers(1, &m_RendererID);

    glGenFramebuffers(1, &m_RendererID);
    glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

    glGenTextures(1, &m_Colorbuffer);
    glBindTexture(GL_TEXTURE_2D, { m_Colorbuffer });
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, { static_cast<GLsizei>(m_Size.x) }, { static_cast<GLsizei>(m_Size.y) }, { 0 }, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, { m_Colorbuffer }, 0);

    glCreateRenderbuffers(1, &m_Renderbuffer);
    // glGenRenderbuffers(1, &m_Renderbuffer);
    // glBindRenderbuffer(GL_RENDERBUFFER, m_Renderbuffer);

    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, { static_cast<GLsizei>(m_Size.x) }, { static_cast<GLsizei>(m_Size.y) });
    // glBindRenderbuffer(GL_RENDERBUFFER, { c_EmptyValue<RendererID> });
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_Renderbuffer);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        return false;
    
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

NAMESPACE_END(Renderer)
