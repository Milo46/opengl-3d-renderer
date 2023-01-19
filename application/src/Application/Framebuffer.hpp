#pragma once

#include "Renderer/RendererCore.hpp"
#include "Renderer/Texture2D.hpp"
#include "Renderer/Bindable.hpp"

#include "Utility/NonCopyable.hpp"

RENDERER_CODE_BEGIN

struct FramebufferProps
{
    glm::vec2 Size{};
};

class Framebuffer
    : public NonCopyable<Framebuffer>,
      public RendererElement,
      public Bindable
{
public:
    DECLARE_CREATABLE(Framebuffer);

public:
    explicit Framebuffer(const FramebufferProps& props) noexcept;
    ~Framebuffer();

    inline const std::shared_ptr<Texture2D>& GetColorbuffer() const noexcept { return m_TextureColorbuffer; }

public:
    bool Initialize() noexcept;

public:
    virtual void Bind() const override;
    virtual void Unbind() const override;

private:
    glm::vec2 m_Size{};
    RendererID m_Renderbuffer{ 0u };
    std::shared_ptr<Texture2D> m_TextureColorbuffer{};
};

DEFINE_DEFAULT_CREATE(Framebuffer);

RENDERER_CODE_END
