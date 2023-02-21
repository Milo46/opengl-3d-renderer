#include "Buffers.hpp"

#include <array>
#include <spdlog/spdlog.h>
#include <glad/glad.h>

NAMESPACE_BEGIN(Renderer)

static constexpr std::size_t _GetSize(const LayoutDataType& type) noexcept
{
    constexpr auto booleanSize{ static_cast<std::size_t>(sizeof(bool))  };
    constexpr auto integerSize{ static_cast<std::size_t>(sizeof(int))   };
    constexpr auto floatSize  { static_cast<std::size_t>(sizeof(float)) };

    switch (type)
    {
    case LayoutDataType::Boolean: return booleanSize * 1u;

    case LayoutDataType::Integer1: return integerSize * 1u;
    case LayoutDataType::Integer2: return integerSize * 2u;
    case LayoutDataType::Integer3: return integerSize * 3u;
    case LayoutDataType::Integer4: return integerSize * 4u;

    case LayoutDataType::Float1: return floatSize * 1u;
    case LayoutDataType::Float2: return floatSize * 2u;
    case LayoutDataType::Float3: return floatSize * 3u;
    case LayoutDataType::Float4: return floatSize * 4u;

    case LayoutDataType::Mat3: return floatSize * 3u * 3u;
    case LayoutDataType::Mat4: return floatSize * 4u * 4u;
    }
}

static constexpr std::size_t _GetComponentCount(const LayoutDataType& type) noexcept
{
    switch (type)
    {
        case LayoutDataType::Boolean:
        case LayoutDataType::Integer1:
        case LayoutDataType::Float1: return 1u;

        case LayoutDataType::Integer2:
        case LayoutDataType::Float2: return 2u;

        case LayoutDataType::Integer3:
        case LayoutDataType::Float3: return 3u;

        case LayoutDataType::Integer4:
        case LayoutDataType::Float4: return 4u;

        case LayoutDataType::Mat3: return 3u * 3u;
        case LayoutDataType::Mat4: return 4u * 4u;
    }
}

BufferElement::BufferElement(const LayoutDataType type, const std::string_view name, const bool normalized)
    : Type{ type }, Name{ name }, Normalized{ normalized}, Size{ _GetSize(type) } {}

std::size_t BufferElement::GetComponentCount() const noexcept
{
    return _GetComponentCount(Type);
}

BufferLayout::BufferLayout(const std::initializer_list<BufferElement>& elements)
    : m_Elements{ elements }
{
    for (auto& bufferElement : m_Elements)
    {
        bufferElement.Offset = m_Stride;
        m_Stride += bufferElement.Size;
    }
}

VertexBuffer::VertexBuffer(const VertexBufferProps& props)
    : m_Props{ props } {}

VertexBuffer::~VertexBuffer()
{
    if (m_RendererID != c_EmptyValue<RendererID>)
        glDeleteBuffers(1, &m_RendererID);
}

bool VertexBuffer::OnInitialize() noexcept
{
    if (m_RendererID != c_EmptyValue<RendererID>)
        glDeleteBuffers(1, &m_RendererID);
    
    glGenBuffers(1, &m_RendererID);
    glBindBuffer(GL_ARRAY_BUFFER, { m_RendererID });
    glBufferData(GL_ARRAY_BUFFER, { static_cast<GLsizeiptr>(m_Props.DataSize * m_Props.VertSize) }, { m_Props.Data }, { static_cast<GLenum>(m_Props.Usage) });

    return true;
}

void VertexBuffer::Bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}

void VertexBuffer::Unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, c_EmptyValue<RendererID>);
}

IndexBuffer::IndexBuffer(const IndexBufferProps& props)
    : m_Props{ props } {}

IndexBuffer::~IndexBuffer()
{
    if (m_RendererID != c_EmptyValue<RendererID>)
        glDeleteBuffers(1, &m_RendererID);
}

bool IndexBuffer::OnInitialize() noexcept
{
    if (m_RendererID != c_EmptyValue<RendererID>)
        glDeleteBuffers(1, &m_RendererID);

    glGenBuffers(1, &m_RendererID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, { m_RendererID });
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        { static_cast<GLsizeiptr>(m_Props.Count * sizeof(uint32_t)) },
        { m_Props.Data },
        { static_cast<GLenum>(m_Props.Usage) }
    );

    return true;
}

void IndexBuffer::Bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
}

void IndexBuffer::Unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, c_EmptyValue<RendererID>);
}

NAMESPACE_END(Renderer)
