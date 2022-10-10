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
    inline static auto Create(const VertexArrayProps& props) noexcept
    {
        return std::make_shared<VertexArray>(props);
    }

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

RENDERER_CODE_END
