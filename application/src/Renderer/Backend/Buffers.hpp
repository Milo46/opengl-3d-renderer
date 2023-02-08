#pragma once

#include "RendererResource.hpp"

#include "Utility/NonCopyable.hpp"

#include <memory>
#include <string>
#include <vector>

NAMESPACE_BEGIN(Renderer)

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

    inline const auto& GetStride() const noexcept { return m_Stride; }
    inline const auto& GetElements() const noexcept { return m_Elements; }

private:
    std::vector<BufferElement> m_Elements{};
    std::size_t m_Stride{ 0u };
};

struct VertexBufferProps
{
    const void* Data{ nullptr };
    std::size_t DataSize{ 0u };
    std::size_t VertSize{ 0u };
    BufferUsage Usage{ BufferUsage::StaticDraw };
    BufferLayout Layout{};
};

class VertexBuffer : public RendererResource<VertexBufferProps>
{
public:
    explicit VertexBuffer(const VertexBufferProps& props);
    ~VertexBuffer();

    inline const auto& GetLayout() const noexcept { return m_Props.Layout; }
    inline const auto& GetSize() const noexcept { return m_Props.DataSize; }

public:
    virtual bool OnInitialize() noexcept override;

public:
    virtual void Bind() const override;
    virtual void Unbind() const override;

public:
    inline virtual RendererID GetResourceHandle() const override { return m_RendererID; }

private:
    RendererID m_RendererID{ c_EmptyValue<RendererID> };
    VertexBufferProps m_Props{};
};

struct IndexBufferProps
{
    const void* Data{ nullptr };
    std::size_t Count{ 0u };
    BufferUsage Usage{ BufferUsage::StaticDraw };
};

class IndexBuffer : public RendererResource<IndexBufferProps>
{
public:
    explicit IndexBuffer(const IndexBufferProps& props);
    ~IndexBuffer();

    inline const auto GetCount() const noexcept { return m_Props.Count; }

public:
    virtual bool OnInitialize() noexcept override;

public:
    virtual void Bind() const override;
    virtual void Unbind() const override;

public:
    inline virtual RendererID GetResourceHandle() const override { return m_RendererID; }

private:
    RendererID m_RendererID{ c_EmptyValue<RendererID> };
    IndexBufferProps m_Props{};
};

NAMESPACE_END(Renderer)
