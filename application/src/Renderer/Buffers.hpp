#pragma once

#include <memory>
#include <string>
#include <vector>

#include "RendererCore.hpp"
#include "Renderer/Bindable.hpp"

#include "Utility/NonCopyable.hpp"

RENDERER_CODE_BEGIN

enum class BufferUsage
{
    None = 0,
    StreamDraw,  StreamRead,  StreamCopy,
    StaticDraw,  StaticRead,  StaticCopy,
    DynamicDraw, DynamicRead, DynamicCopy,
    EnumEnd,
};

enum class LayoutDataType
{
    None = 0,
    Boolean,
    Integer1, Integer2, Integer3, Integer4,
    Float1, Float2, Float3, Float4,
    Mat3, Mat4,
    EnumEnd,
};

struct BufferElement
{
    std::string_view Name{};
    LayoutDataType Type{ LayoutDataType::None };

    std::size_t Size{ 0u };
    std::size_t Offset{ 0u };
    bool Normalized{ false };

    BufferElement(
        const LayoutDataType type,
        const std::string_view name,
        const bool normalized = false);
    
    std::size_t GetComponentCount() const noexcept;
};

class BufferLayout
{
public:
    BufferLayout() = default;
    BufferLayout(const std::initializer_list<BufferElement>& elements);

    inline const auto  GetStride() const noexcept   { return m_Stride; }
    inline const auto& GetElements() const noexcept { return m_Elements; }

private:
    void CalculateData() noexcept;

private:
    std::vector<BufferElement> m_Elements{};
    std::size_t m_Stride{ 0u };
};

struct VertexBufferProps
{
    const void* Data{ nullptr };
    const std::size_t Size{ 0u };
    const BufferUsage Usage{ BufferUsage::StaticDraw };
    const BufferLayout Layout{};
};

class VertexBuffer
    : public NonCopyable<VertexBuffer>,
      public RendererElement,
      public Bindable
{
public:
    DECLARE_CREATABLE(VertexBuffer);

public:
    explicit VertexBuffer(const VertexBufferProps& props);
    ~VertexBuffer();

    inline const auto& GetLayout() const noexcept { return m_Layout; }

public:
    virtual void Bind() const override;
    virtual void Unbind() const override;

private:
    BufferLayout m_Layout{};
};

struct IndexBufferProps
{
    const void* Data{ nullptr };
    const std::size_t Count{ 0u };
    const BufferUsage Usage{ BufferUsage::StaticDraw };
};

class IndexBuffer
    : public NonCopyable<IndexBuffer>,
      public RendererElement,
      public Bindable
{
public:
    DECLARE_CREATABLE(IndexBuffer);

public:
    explicit IndexBuffer(const IndexBufferProps& props);
    ~IndexBuffer();

    inline const auto GetCount() const noexcept { return m_Count; }

public:
    virtual void Bind() const override;
    virtual void Unbind() const override;

private:
    std::size_t m_Count{ 0u };
};

DEFINE_DEFAULT_CREATE(VertexBuffer);
DEFINE_DEFAULT_CREATE(IndexBuffer);

RENDERER_CODE_END
