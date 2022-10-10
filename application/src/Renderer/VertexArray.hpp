#pragma once

#include "Renderer/RendererCore.hpp"
#include "Renderer/Buffers.hpp"

RENDERER_CODE_BEGIN

struct VertexArrayProps
{
    std::shared_ptr<VertexBuffer> VertexBuffer;
    std::shared_ptr<IndexBuffer> IndexBuffer;
};

class VertexArray
    : public NonCopyable<VertexArray>,
      public RendererElement,
      public Bindable
{
public:
    DECLARE_CREATABLE(VertexArray);

public:
    explicit VertexArray(const VertexArrayProps& props);
    ~VertexArray();

    void SetVertexBuffer(std::shared_ptr<VertexBuffer> vertexBuffer);
    void SetIndexBuffer(std::shared_ptr<IndexBuffer> indexBuffer);

    inline const auto& GetVertexBuffer() const noexcept { return m_VertexBuffer; }
    inline const auto& GetIndexBuffer() const noexcept { return m_IndexBuffer; }

public:
    virtual void Bind() const override;
    virtual void Unbind() const override;

private:
    std::shared_ptr<VertexBuffer> m_VertexBuffer;
    std::shared_ptr<IndexBuffer> m_IndexBuffer;
};

DEFINE_DEFAULT_CREATE(VertexArray);

RENDERER_CODE_END
