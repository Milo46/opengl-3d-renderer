#pragma once

#include "Renderer/RendererCore.hpp"
#include "Renderer/Texture2D.hpp"

#include "Utility/NonCopyable.hpp"

RENDERER_CODE_BEGIN

struct FramebufferProps
{
    glm::vec2 Size{};
};

class Framebuffer : public RendererResource
{
public:
    DECLARE_CREATABLE(Framebuffer);

public:
    explicit Framebuffer(const FramebufferProps& props) noexcept;
    ~Framebuffer();

    inline const std::shared_ptr<Texture2D>& GetColorbuffer() const noexcept { return m_TextureColorbuffer; }

public:
    virtual bool OnInitialize() noexcept override;

public:
    virtual void Bind() const override;
    virtual void Unbind() const override;

public:
    inline virtual RendererID GetResourceHandle() const override { return m_RendererID; }

private:
    RendererID m_RendererID{ c_EmptyValue<RendererID> };
    glm::vec2 m_Size{};
    RendererID m_Renderbuffer{ 0u };
    std::shared_ptr<Texture2D> m_TextureColorbuffer{};
};

DEFINE_DEFAULT_CREATE(Framebuffer);

RENDERER_CODE_END
