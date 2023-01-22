#pragma once

#include "Renderer/RendererCore.hpp"
#include "Renderer/Buffers.hpp"

RENDERER_CODE_BEGIN

struct VertexArrayProps
{
    std::shared_ptr<VertexBuffer> VertexBufferPtr;
    std::shared_ptr<IndexBuffer> IndexBufferPtr;
};

class VertexArray : public RendererResource
{
public:
    DECLARE_CREATABLE(VertexArray);

public:
    explicit VertexArray(const VertexArrayProps& props);
    ~VertexArray();

    inline const auto& GetVertexBuffer() const noexcept { return m_VertexBuffer; }
    inline const auto& GetIndexBuffer() const noexcept { return m_IndexBuffer; }

public:
    virtual bool OnInitialize() noexcept;

public:
    virtual void Bind() const override;
    virtual void Unbind() const override;

public:
    inline virtual RendererID GetResourceHandle() const override { return m_RendererID; }

private:
    RendererID m_RendererID{ c_EmptyValue<RendererID> };
    std::shared_ptr<VertexBuffer> m_VertexBuffer;
    std::shared_ptr<IndexBuffer> m_IndexBuffer;
};

DEFINE_DEFAULT_CREATE(VertexArray);

RENDERER_CODE_END
