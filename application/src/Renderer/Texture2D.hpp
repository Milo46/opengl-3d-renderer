#pragma once

#include "Renderer/RendererCore.hpp"

#include "Utility/NonCopyable.hpp"

#include <glm/glm.hpp>

#include <string>

RENDERER_CODE_BEGIN

enum class TextureWrapping
{
    None = 0,

    Repeat,
    MirroredRepeat,
    ClampToEdge,
    ClampToBorder,

    EnumEnd,
};

enum class TextureFiltering
{
    None = 0,

    Nearest,
    Linear,

    EnumEnd,
};

struct Texture2DProps
{
    glm::vec2 Size{ 0u, 0u };
    TextureWrapping Wrapping{ TextureWrapping::Repeat };
    TextureFiltering Filtering{ TextureFiltering::Linear };
    std::string Filepath{ "" };
};

class Texture2D : public RendererResource
{
public:
    DECLARE_CREATABLE(Texture2D);

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

DEFINE_DEFAULT_CREATE(Texture2D);

RENDERER_CODE_END
