#include "VertexArray.hpp"

#include <GLAD/glad.h>

#include <spdlog/spdlog.h>

RENDERER_CODE_BEGIN

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

        default:
            return static_cast<RendererEnum>(-1);
        }
    }
}

VertexArray::VertexArray(const VertexArrayProps& props)
{
    glCreateVertexArrays(1, &m_RendererID);

    VertexArray::SetVertexBuffer(props.VertexBuffer);
    VertexArray::SetIndexBuffer(props.IndexBuffer);
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &m_RendererID);
}

void VertexArray::SetVertexBuffer(std::shared_ptr<VertexBuffer> vertexBuffer)
{
    if (!vertexBuffer.get() || vertexBuffer->GetLayout().GetElements().empty()) return;

    VertexArray::Bind();
    vertexBuffer->Bind();

    std::size_t attribIndex{ 0u };
    const auto& layout{ vertexBuffer->GetLayout() };
    for (const auto& element : layout.GetElements())
    {
        glEnableVertexAttribArray(attribIndex);
        glVertexAttribPointer(attribIndex++, element.GetComponentCount(), Internal::GetGLLayoutDataType(element.Type),
            element.Normalized ? GL_TRUE : GL_FALSE, layout.GetStride(),
            reinterpret_cast<const void*>(static_cast<uintptr_t>(element.Offset)));
    }

    m_VertexBuffer = vertexBuffer;
}

void VertexArray::SetIndexBuffer(std::shared_ptr<IndexBuffer> indexBuffer)
{
    if (!indexBuffer.get() || !indexBuffer->GetCount()) return;

    VertexArray::Bind();
    indexBuffer->Bind();

    m_IndexBuffer = indexBuffer;
}

void VertexArray::Bind() const
{
    glBindVertexArray(m_RendererID);
}

void VertexArray::Unbind() const
{
    glBindVertexArray(::Renderer::c_EmptyID);
}

RENDERER_CODE_END
