#pragma once

#include "RendererCore.hpp"
#include "Bindable.hpp"

#include <string>
#include <xtr1common>
#include <filesystem>
#include <unordered_map>

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
    const std::unordered_map<ShaderType, std::string> Filepaths;
    const bool CompileAndLink{ true };
};

class Shader : public Bindable
{
public:
    static constexpr auto c_ShaderCount{ GetShaderTypeCount<std::size_t>() };
    static constexpr std::int32_t c_InvalidUniformLocation{ -1 };

public:
    bool LoadFromText(const ShaderType type, const std::string_view source, bool recompile = false);
    bool LoadFromFile(const ShaderType type, const std::string_view filepath, bool recompile = false);

    bool Compile();
    bool Link();

private:
    explicit Shader(const ShaderProps& props);
    virtual ~Shader();

private:
    std::string LoadShaderInfoLog(const RendererID id) const;
    std::string LoadProgramInfoLog(const RendererID id) const;

    bool CompileShader(const RendererID id);
    bool CompielShader(const ShaderType type);

private:
    RendererID m_RendererID{ 0u };
    RendererID m_Shaders[Shader::c_ShaderCount]{ 0u };

    std::string m_InfoLog{};
};

RENDERER_CODE_END
