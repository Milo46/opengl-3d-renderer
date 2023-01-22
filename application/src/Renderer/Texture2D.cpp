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
const static auto c_DataFormat    { GL_RGBA  };

Texture2D::Texture2D(const Texture2DProps& props)
    : m_Size{ props.Size }, m_Wrapping{ props.Wrapping }, m_Filtering{ props.Filtering }, m_Filepath{ props.Filepath } {}

Texture2D::~Texture2D()
{
    if (m_RendererID != c_EmptyValue<RendererID>)
        glDeleteTextures(1, &m_RendererID);
}

bool Texture2D::OnInitialize() noexcept
{
    if (m_RendererID != c_EmptyValue<RendererID>)
        glDeleteTextures(1, &m_RendererID);
    
    glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
    glBindTexture(GL_TEXTURE_2D, { m_RendererID });

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, { static_cast<GLint>(Internal::GetGLTextureWrapping(m_Wrapping)) });
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, { static_cast<GLint>(Internal::GetGLTextureWrapping(m_Wrapping)) });

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, { static_cast<GLint>(Internal::GetGLTextureFiltering(m_Filtering)) });
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, { static_cast<GLint>(Internal::GetGLTextureFiltering(m_Filtering)) });

    if (!m_Filepath.empty())
    {
        int width{}, height{}, channels{};
        stbi_set_flip_vertically_on_load(1);

        auto* data{ stbi_load(m_Filepath.c_str(), &width, &height, &channels, 0) };
        if (!data)
        {
            if (stbi_failure_reason())
                spdlog::error("[stbi_image]: {}: {}", stbi_failure_reason(), m_Filepath);
            else
                spdlog::error("[stbi_image]: failed without any reason: {}", m_Filepath);

            return false;
        }

        m_Size.x = { static_cast<float>(width)  };
        m_Size.y = { static_cast<float>(height) };

        const auto internalFormat{ channels == 4 ? GL_RGBA8 : channels == 3 ? GL_RGB8 : c_InvalidValue<RendererEnum> };
        const auto dataFormat    { channels == 4 ? GL_RGBA  : channels == 3 ? GL_RGB  : c_InvalidValue<RendererEnum> };

        // #1. Weird, idk why.
        // glTexImage2D(GL_TEXTURE_2D, 0, { static_cast<GLint>(internalFormat) }, { width }, { height }, 0, { dataFormat }, GL_UNSIGNED_BYTE, data);
        // glGenerateMipmap(GL_TEXTURE_2D);

        // #2. Maybe better method.
        glTextureStorage2D({ m_RendererID }, 1, { internalFormat }, { width }, { height });
        glTextureSubImage2D({ m_RendererID }, 0, 0, 0, { width }, { height }, { dataFormat }, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);
    }
    else
    {
        glTextureStorage2D(m_RendererID, 1, { c_InternalFormat }, { static_cast<int>(m_Size.x) }, { static_cast<int>(m_Size.y) });
        glTexImage2D(GL_TEXTURE_2D, 0, { c_InternalFormat }, { static_cast<int>(m_Size.x) }, { static_cast<int>(m_Size.y) }, 0, { c_DataFormat }, GL_UNSIGNED_BYTE, nullptr);
        // glGenerateMipmap(GL_TEXTURE_2D);
    }

    return true;
}

void Texture2D::Bind() const
{
    glBindTexture(GL_TEXTURE_2D, m_RendererID);
}

void Texture2D::Unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0u);
}

RENDERER_CODE_END
