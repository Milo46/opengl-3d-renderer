#pragma once

#include "RendererResource.hpp"

#include "Texture2D.hpp"

#include <spdlog/spdlog.h>

NAMESPACE_BEGIN(Renderer)

struct FramebufferProps
{
    glm::vec2 Size{};
};

class Framebuffer : public RendererResource<FramebufferProps>
{
public:
    explicit Framebuffer(const FramebufferProps& props);
    ~Framebuffer();

    inline const auto& GetSize() const { return m_Size; }
    inline const auto& GetColorbufferID() const { return m_Colorbuffer; }

public:
    virtual bool OnInitialize() noexcept override;

public:
    virtual void Bind() const override;
    virtual void Unbind() const override;

public:
    inline virtual RendererID GetResourceHandle() const override { return m_RendererID; }

private:
    RendererID m_RendererID{ c_EmptyValue<RendererID> };
    RendererID m_Colorbuffer{ c_EmptyValue<RendererID> };
    RendererID m_Renderbuffer{ c_EmptyValue<RendererID> };
    glm::vec2 m_Size{};
};

NAMESPACE_END(Renderer)
