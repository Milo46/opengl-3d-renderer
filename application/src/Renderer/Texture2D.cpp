#include "Texture2D.hpp"

#include <spdlog/spdlog.h>

#include <array>
#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

RENDERER_CODE_BEGIN

namespace Internal
{
    template<typename _EnumClass>
    constexpr static inline auto EnumIndex(const _EnumClass value) noexcept { return static_cast<std::size_t>(value) - 1u; }

    template<typename _EnumClass>
    constexpr static inline auto IsEnumValid(const _EnumClass value) noexcept { return _EnumClass::None < value && _EnumClass::EnumEnd < value; }

    constexpr auto c_InvalidTextureWrapping{ static_cast<RendererEnum>(-1) };
    constexpr static inline auto GetGLTextureWrapping(const TextureWrapping wrapping) noexcept
    {
        std::array<RendererEnum, static_cast<std::size_t>(TextureWrapping::EnumEnd)> data{
            GL_REPEAT, GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER,
        };

        return IsEnumValid(wrapping) ? data.at(EnumIndex(wrapping)) : c_InvalidTextureWrapping;
    }

    constexpr auto c_InvalidTextureFiltering{ static_cast<RendererEnum>(-1) };
    constexpr static inline auto GetGLTextureFiltering(const TextureFiltering filtering) noexcept
    {
        std::array<RendererEnum, static_cast<std::size_t>(TextureFiltering::EnumEnd)> data{
            GL_NEAREST, GL_LINEAR,
        };

        return IsEnumValid(filtering) ? data.at(EnumIndex(filtering)) : c_InvalidTextureWrapping;
    }
}

const static auto c_InternalFormat{ GL_RGBA8 };
const static auto c_DataFormat{ GL_RGBA };

Texture2D::Texture2D(const Texture2DProps& props)
    : m_Size{ props.Size }, m_Wrapping{ props.Wrapping }, m_Filtering{ props.Filtering }
{
    glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
    glTextureStorage2D(m_RendererID, 1, c_InternalFormat, static_cast<RendererSizei>(m_Size.x), static_cast<RendererSizei>(m_Size.y));

    glTexImage2D(GL_TEXTURE_2D, 0, c_InternalFormat, static_cast<RendererSizei>(m_Size.x), static_cast<RendererSizei>(m_Size.y), 0, c_DataFormat, GL_UNSIGNED_BYTE, nullptr);

    glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, Internal::GetGLTextureFiltering(m_Filtering));
    glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, Internal::GetGLTextureFiltering(m_Filtering));

    glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, Internal::GetGLTextureWrapping(m_Wrapping));
    glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, Internal::GetGLTextureWrapping(m_Wrapping));
}

Texture2D::~Texture2D()
{
    glDeleteTextures(1, &m_RendererID);
}

void Texture2D::SetData(void* data, std::size_t size)
{
    if (size != m_Size.x * m_Size.y * 4u)
    {
        spdlog::error("[OpenGL](Texture2D) Texture data must be the entire texture!");
        return;
    }

    glTextureSubImage2D(m_RendererID, 0u, 0u, 0u, m_Size.x, m_Size.y, c_DataFormat, GL_UNSIGNED_BYTE, data);
}

void Texture2D::Bind() const
{
    glBindTexture(GL_TEXTURE_2D, m_RendererID);
}

void Texture2D::Unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0u);
}

bool Texture2D::LoadFilepath(const std::string& path)
{
    int width{}, height{}, channels{};
    stbi_set_flip_vertically_on_load(1);

    auto* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
    spdlog::info("{} {} {}", width, height, channels);

    if (!data)
    {
        if (stbi_failure_reason())
            spdlog::error("[stbi_image]: {}: {}", stbi_failure_reason(), path);
        else
            spdlog::error("[stbi_image]: failed without any reason: {}", path);

        return false;
    }

    m_Size = { width, height };
    auto internalFormat{ channels == 4 ? GL_RGBA8 : channels == 3 ? GL_RGB8 : 0 };
    auto dataFormat    { channels == 4 ? GL_RGBA  : channels == 3 ? GL_RGB  : 0 };

    if (m_RendererID) glDeleteTextures(1, &m_RendererID);
    glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);

    glTextureStorage2D(m_RendererID, 1, internalFormat, width, height);
    glTextureSubImage2D(m_RendererID, 0, 0, 0, width, height, dataFormat, GL_UNSIGNED_BYTE, data);

    glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, Internal::GetGLTextureFiltering(m_Filtering));
    glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, Internal::GetGLTextureFiltering(m_Filtering));

    glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, Internal::GetGLTextureWrapping(m_Wrapping));
    glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, Internal::GetGLTextureWrapping(m_Wrapping));

    stbi_image_free(data);

    return true;
}

RENDERER_CODE_END
