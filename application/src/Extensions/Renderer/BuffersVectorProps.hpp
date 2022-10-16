#pragma once

#include "Renderer/Buffers.hpp"

namespace Renderer
{
    namespace Extension
    {
        struct VertexBufferPropsVector
        {
            const std::vector<float> VectorData{};
            const BufferUsage Usage{ BufferUsage::StaticDraw };
            const BufferLayout Layout{};
        };

        struct IndexBufferPropsVector
        {
            const std::vector<unsigned int> VectorData{};
            const BufferUsage Usage{ BufferUsage::StaticDraw };
        };
    }

    template<>
    inline auto Create<::Renderer::VertexBuffer, Extension::VertexBufferPropsVector>(const Extension::VertexBufferPropsVector& props) noexcept -> std::shared_ptr<VertexBuffer>
    {
        return Create<VertexBuffer>({
            .Data   = props.VectorData.data(),
            .Size   = props.VectorData.size() * sizeof(decltype(props.VectorData)::value_type),
            .Usage  = props.Usage,
            .Layout = props.Layout,
        });
    }

    template<>
    inline auto Create<::Renderer::IndexBuffer, Extension::IndexBufferPropsVector>(const Extension::IndexBufferPropsVector& props) noexcept -> std::shared_ptr<IndexBuffer>
    {
        return Create<IndexBuffer>({
            .Data  = props.VectorData.data(),
            .Count = props.VectorData.size(),
            .Usage = props.Usage,
        });
    }
}
