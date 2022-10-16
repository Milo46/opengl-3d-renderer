#pragma once

#include "Renderer/RendererCore.hpp"
#include "Renderer/Bindable.hpp"

#include "Utility/NonCopyable.hpp"

#include <glm/glm.hpp>

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
    const glm::vec2 Size{ 1u, 1u };

    const TextureWrapping Wrapping{ TextureWrapping::Repeat };
    const TextureFiltering Filtering{ TextureFiltering::Linear };
};

class Texture2D
    : public NonCopyable<Texture2D>,
      public RendererElement,
      public Bindable
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
    virtual void Bind() const override;
    virtual void Unbind() const override;

private:
    glm::vec2 m_Size{};

    TextureWrapping m_Wrapping{};
    TextureFiltering m_Filtering{};
};

DEFINE_DEFAULT_CREATE(Texture2D);

RENDERER_CODE_END
