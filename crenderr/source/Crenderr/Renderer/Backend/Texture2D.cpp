#include "Texture2D.hpp"

#include <spdlog/spdlog.h>

#include <algorithm>

#include <array>
#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

NAMESPACE_BEGIN(Renderer)

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

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, { static_cast<GLint>(m_Wrapping) });
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, { static_cast<GLint>(m_Wrapping) });

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, { static_cast<GLint>(m_Filtering) });
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, { static_cast<GLint>(m_Filtering) });

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
        // glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);
    }
    else
    {
        const auto dataSize{ static_cast<std::size_t>(m_Size.x * m_Size.y * 4u) };
        auto data = new unsigned char[dataSize]{};
        std::fill(data, data + static_cast<std::ptrdiff_t>(dataSize), 255u);

        glTextureStorage2D(m_RendererID, 1, { c_InternalFormat }, { static_cast<int>(m_Size.x) }, { static_cast<int>(m_Size.y) });
        glTextureSubImage2D(m_RendererID, 0, 0, 0, { static_cast<int>(m_Size.x) }, { static_cast<int>(m_Size.y) }, { c_DataFormat }, GL_UNSIGNED_BYTE, data);
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

NAMESPACE_END(Renderer)
