#pragma once

#include "RendererCore.hpp"
#include "Bindable.hpp"

#include <unordered_map>
#include <xtr1common>
#include <filesystem>
#include <memory>
#include <string>
#include <array>

// #include <glm/glm.hpp>
// #include <glm/gtc/type_ptr.hpp>

#include "Utility/FileManager.hpp"

RENDERER_CODE_BEGIN

enum class ShaderType
{
    None = 0,
    Vertex = 1, Fragment, Geometry, Compute, TessControl, Evaluation,
    EnumEnd,
};

template<typename _Integral>
inline constexpr auto GetShaderTypeCount() noexcept
{
    static_assert(std::is_integral_v<_Integral>);
    return static_cast<_Integral>(ShaderType::EnumEnd);
}

struct ShaderProps
{
    const std::unordered_map<ShaderType, FileManager> Sources{};
    const bool Initialize{ false };
};

struct Shader
    : public RendererElement,
      public Bindable
{
    friend struct ShaderDataExtractor;

public:
    static constexpr auto c_ShaderCount{ GetShaderTypeCount<std::size_t>() };

public:
    inline static auto Create(const ShaderProps& props) noexcept
    {
        return std::make_shared<Shader>(props);
    }

public:
    explicit Shader(const ShaderProps& props) noexcept;
    ~Shader() noexcept;

    bool LoadSource(const ShaderType type, const std::string& source) noexcept;
    bool LoadSource(const ShaderType type, FileManager& source) noexcept;
    bool LoadSource(const ShaderType type, const FileManager& source) noexcept;

    bool Compile() noexcept;
    bool Link() noexcept;

public:
    virtual void Bind() const override;
    virtual void Unbind() const override;

private:
    bool InternalLoadSource(const ShaderType type, const FileManager& source) noexcept;
    bool InternalCompileShader(const std::size_t index);

private:
    std::array<RendererID, Shader::c_ShaderCount> m_Handles{ 0u };
    std::array<FileManager, Shader::c_ShaderCount> m_Sources{};
    std::array<bool, Shader::c_ShaderCount> m_Compiled{ false };
};

struct ShaderData
{
    const RendererID ID;
    const std::string Type;
    const FileManager Source;
};

struct ShaderDataExtractor
{
    std::vector<ShaderData> ShaderData{};
    const std::shared_ptr<Shader>& Ref;

    explicit ShaderDataExtractor(const std::shared_ptr<Shader>& shader) noexcept;
};

RENDERER_CODE_END
