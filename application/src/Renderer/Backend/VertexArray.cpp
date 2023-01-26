#include "VertexArray.hpp"

#include <glad/glad.h>

#include <spdlog/spdlog.h>

NAMESPACE_BEGIN(Renderer)

namespace Internal
{
    constexpr inline static RendererEnum GetGLLayoutDataType(const LayoutDataType type) noexcept
    {
        switch (type)
        {
        case LayoutDataType::Boolean: return GL_BOOL;

        case LayoutDataType::Integer1:
        case LayoutDataType::Integer2:
        case LayoutDataType::Integer3:
        case LayoutDataType::Integer4: return GL_INT;

        case LayoutDataType::Float1:
        case LayoutDataType::Float2:
        case LayoutDataType::Float3:
        case LayoutDataType::Float4:
        case LayoutDataType::Mat3:
        case LayoutDataType::Mat4: return GL_FLOAT;

        default: return c_InvalidValue<RendererEnum>;
        }
    }
}

VertexArray::VertexArray(const VertexArrayProps& props)
    : m_VertexBuffer{ props.VertexBufferPtr }, m_IndexBuffer{ props.IndexBufferPtr } {}

VertexArray::~VertexArray()
{
    if (m_RendererID != c_EmptyValue<RendererID>)
        glDeleteVertexArrays(1, &m_RendererID);
}

bool VertexArray::OnInitialize() noexcept
{
    if (m_RendererID != c_EmptyValue<RendererID>)
        glDeleteVertexArrays(1, &m_RendererID);

    glCreateVertexArrays(1, &m_RendererID);
    glBindVertexArray(m_RendererID);

    if (!m_VertexBuffer.get() || m_VertexBuffer->GetLayout().GetElements().empty()) return false;

    m_VertexBuffer->Bind();

    const auto& layoutElements{ m_VertexBuffer->GetLayout().GetElements() };
    const auto& stride{ m_VertexBuffer->GetLayout().GetStride() };

    for (std::size_t i = 0u; i < layoutElements.size(); ++i)
    {
        const auto& element{ layoutElements[i] };

        glEnableVertexAttribArray({ static_cast<GLuint>(i) });
        glVertexAttribPointer(
            { static_cast<GLuint>(i) },
            { static_cast<GLint>(element.GetComponentCount()) },
            { Internal::GetGLLayoutDataType(element.Type) },
            { static_cast<GLboolean>(element.Normalized ? GL_TRUE : GL_FALSE) },
            { static_cast<GLsizei>(stride) },
            { reinterpret_cast<const void*>(element.Offset) }
        );
    }

    if (!m_IndexBuffer.get() || !m_IndexBuffer->GetCount())
    {
        spdlog::warn("Initialized a VertexArray without indices! [id={}]", m_RendererID);
        return true;
    }

    m_IndexBuffer->Bind();

    return true;
}

void VertexArray::Bind() const
{
    glBindVertexArray(m_RendererID);
}

void VertexArray::Unbind() const
{
    glBindVertexArray(c_EmptyValue<RendererID>);
}

NAMESPACE_END(Renderer)
