#pragma once

#include "RendererResource.hpp"

#include "Utility/NonCopyable.hpp"

#include <glm/glm.hpp>
#include <string>

NAMESPACE_BEGIN(Renderer)

enum class TextureWrapping : RendererEnum
{
    Repeat         = 0x2901,
    MirroredRepeat = 0x8370,
    ClampToEdge    = 0x812F,
    ClampToBorder  = 0x812D,
};

enum class TextureFiltering : RendererEnum
{
    Nearest = 0x2600,
    Linear  = 0x2601,
};

struct Texture2DProps
{
    glm::vec2 Size{ 0u, 0u };
    TextureWrapping Wrapping{ TextureWrapping::Repeat };
    TextureFiltering Filtering{ TextureFiltering::Linear };
    std::string Filepath{ "" };
};

class Texture2D : public RendererResource<Texture2DProps>
{
public:
    explicit Texture2D(const Texture2DProps& props);
    ~Texture2D();

    inline const auto& GetSize() const noexcept { return m_Size; }
    inline auto GetWidth() const noexcept { return m_Size.x; }
    inline auto GetHeight() const noexcept { return m_Size.y; }

public:
    virtual bool OnInitialize() noexcept override;

public:
    virtual void Bind() const override;
    virtual void Unbind() const override;

public:
    inline virtual RendererID GetResourceHandle() const override { return m_RendererID; }

private:
    RendererID m_RendererID{ c_EmptyValue<RendererID> };

    std::string m_Filepath{};
    glm::vec2 m_Size{};

    TextureWrapping m_Wrapping{};
    TextureFiltering m_Filtering{};
};

NAMESPACE_END(Renderer)
