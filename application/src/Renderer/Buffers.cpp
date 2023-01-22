#include "Buffers.hpp"

#include <array>
#include <spdlog/spdlog.h>
#include <glad/glad.h>

RENDERER_CODE_BEGIN

#define RENDERER_ENUM_DEFINITIONS(_ClassName, _OutputType, ...) \
    constexpr static auto c_Invalid##_ClassName{ static_cast<RendererEnum>(-1) }; \
    constexpr static inline auto _ClassName##Index(const _ClassName a) noexcept { return static_cast<std::size_t>(a) - 1u; } \
    constexpr static inline auto Is##_ClassName##Valid(const _ClassName a) noexcept { return _ClassName::None < a && a < _ClassName::EnumEnd; } \
    constexpr static inline auto GetGL##_ClassName(const _ClassName a) noexcept \
    { \
        const std::array<_OutputType, static_cast<std::size_t>(_ClassName::EnumEnd)> data{ \
            __VA_ARGS__ \
        }; \
        return Is##_ClassName##Valid(a) ? data.at(_ClassName##Index(a)) : []() { \
            spdlog::critical("[Internal] Invalid "#_ClassName" argument!"); \
            return c_Invalid##_ClassName; \
        }(); \
    }

namespace Internal
{
    RENDERER_ENUM_DEFINITIONS(BufferUsage, RendererEnum,
        GL_STREAM_DRAW,  GL_STREAM_READ,  GL_STREAM_COPY,
        GL_STATIC_DRAW,  GL_STATIC_READ,  GL_STATIC_COPY,
        GL_DYNAMIC_DRAW, GL_DYNAMIC_READ, GL_DYNAMIC_COPY,
    );

    constexpr static auto c_SizeofBoolean{ sizeof(bool)  };
    constexpr static auto c_SizeofInteger{ sizeof(int)   };
    constexpr static auto c_SizeofFloat  { sizeof(float) };

    RENDERER_ENUM_DEFINITIONS(LayoutDataType, std::size_t,
        c_SizeofBoolean * 1u,
        c_SizeofInteger * 1u, c_SizeofInteger * 2u, c_SizeofInteger * 3u, c_SizeofInteger * 4u,
        c_SizeofFloat   * 1u, c_SizeofFloat   * 2u, c_SizeofFloat   * 3u, c_SizeofFloat   * 4u,

        c_SizeofFloat * 3u * 3u, c_SizeofFloat * 4u * 4u,
    );

    constexpr static inline std::size_t GetComponentCount(const LayoutDataType type) noexcept
    {
        const std::array<std::size_t, static_cast<std::size_t>(LayoutDataType::EnumEnd)> data{
            1u,
            1u, 2u, 3u, 4u,
            1u, 2u, 3u, 4u,

            3u * 3u,
            4u * 4u,
        };

        return IsLayoutDataTypeValid(type) ? data.at(LayoutDataTypeIndex(type)) : []() {
            spdlog::critical("[Internal] Invalid LayoutDataType argument!");
            return c_InvalidLayoutDataType;
        }();
    }
}

BufferElement::BufferElement(const LayoutDataType type, const std::string_view name, const bool normalized)
    : Type{ type }, Name{ name }, Normalized{ normalized}, Size{ Internal::GetGLLayoutDataType(type) } {}

std::size_t BufferElement::GetComponentCount() const noexcept
{
    return Internal::GetComponentCount(BufferElement::Type);
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
    glBufferData(GL_ARRAY_BUFFER, { static_cast<GLsizeiptr>(m_Props.Size) }, { m_Props.Data }, { Internal::GetGLBufferUsage(m_Props.Usage) });

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
        { Internal::GetGLBufferUsage(m_Props.Usage) }
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

RENDERER_CODE_END
