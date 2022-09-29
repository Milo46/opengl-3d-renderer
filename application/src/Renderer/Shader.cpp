#include "Shader.hpp"

#include <spdlog/spdlog.h>

#include <optional>
#include <fstream>
#include <sstream>
#include <array>
#include <map>

/*
    TODO: - test if all internal and opengl static functions are exception safe!
*/

RENDERER_CODE_BEGIN

// Move those macros somewhere else!!
#define break_if(_Expression) if (_Expression) break
#define continue_if(_Expression) if (_Expression) continue

namespace Internal
{
    constexpr auto c_InvalidGLShader{ static_cast<RendererEnum>(-1) };
    constexpr auto c_InvalidGLShaderString{ static_cast<const char*>("InvalidShader") };

    constexpr static inline std::size_t ShaderTypeIndex(const ShaderType type) noexcept
    {
        return static_cast<std::size_t>(type);
    }

    constexpr static inline bool IsShaderTypeValid(const ShaderType type) noexcept
    {
        return ShaderType::None < type && type < ShaderType::EnumEnd;
    }

    constexpr static RendererEnum GetGLShaderType(const ShaderType type) noexcept
    {
        const std::array<RendererEnum, static_cast<std::size_t>(ShaderType::EnumEnd)> glShaderNames{
            GL_VERTEX_SHADER,          // <- ShaderType::Vertex
            GL_FRAGMENT_SHADER,        // <- ShaderType::Fragment
            GL_GEOMETRY_SHADER,        // <- ShaderType::Geometry
            GL_COMPUTE_SHADER,         // <- ShaderType::Compute
            GL_TESS_CONTROL_SHADER,    // <- ShaderType::TessControl
            GL_TESS_EVALUATION_SHADER, // <- ShaderType::Evaluation
        };

        return IsShaderTypeValid(type) ? glShaderNames.at(ShaderTypeIndex(type) - 1u) : c_InvalidGLShader;
    }

    constexpr static const char* GetShaderTypeString(const ShaderType type) noexcept
    {
        const std::array<const char*, static_cast<std::size_t>(ShaderType::EnumEnd)> glShaderStrings{
            "VertexShader",         // <- ShaderType::Vertex
            "FragmentShader",       // <- ShaderType::Fragment
            "GeometryShader",       // <- ShaderType::Geometry
            "ComputeShader",        // <- ShaderType::Compute
            "TessControlShader",    // <- ShaderType::TessControl
            "TessEvaluationShader", // <- ShaderType::Evaluation
        };

        return IsShaderTypeValid(type) ? glShaderStrings.at(ShaderTypeIndex(type) - 1u) : c_InvalidGLShaderString;
    }

    std::string LoadFileContent(const std::string_view filepath) noexcept
    {
        std::ifstream file{ filepath.data() };
        if (!file.is_open())
        {
            spdlog::warn("[Internal] Load file content failed (filepath: {})!", filepath.data());
            return {};
        }

        std::stringstream buffer{};
        buffer << file.rdbuf();

        return buffer.str();
    }
}

namespace OpenGL
{
    enum class ShaderParameter : RendererEnum
    {
        ShaderType         = GL_SHADER_TYPE,
        DeleteStatus       = GL_DELETE_STATUS,
        CompileStatus      = GL_COMPILE_STATUS,
        InfoLogLength      = GL_INFO_LOG_LENGTH,
        ShaderSourceLength = GL_SHADER_SOURCE_LENGTH,
    };

    enum class ProgramParameter : RendererEnum
    {
        DeleteStatus             = GL_DELETE_STATUS,
        LinkStatus               = GL_LINK_STATUS,
        ValidateStatus           = GL_VALIDATE_STATUS,
        InfoLogLength            = GL_INFO_LOG_LENGTH,
        AttachedShaders          = GL_ATTACHED_SHADERS,
        ActiveAttributes         = GL_ACTIVE_ATTRIBUTES,
        ActiveAttributeMaxLength = GL_ACTIVE_ATTRIBUTE_MAX_LENGTH,
        ActiveUniforms           = GL_ACTIVE_UNIFORMS,
        ActiveUniformsMaxLength  = GL_ACTIVE_UNIFORM_MAX_LENGTH,
    };

    static GLint GetShaderParameter(const RendererID id, const ShaderParameter parameter) noexcept
    {
        static GLint status{};
        glGetShaderiv(id, static_cast<RendererEnum>(parameter), &status);

        return status;
    }

    static GLint GetProgramParameter(const RendererID id, const ProgramParameter parameter) noexcept
    {
        static GLint status{};
        glGetProgramiv(id, static_cast<RendererEnum>(parameter), &status);

        return status;
    }

    static inline bool DoesProgramExist(const RendererID id) noexcept
    {
        return !(GetProgramParameter(id, ProgramParameter::ValidateStatus) == GL_INVALID_VALUE);
    }

    static bool DoesShaderExist(const RendererID id) noexcept
    {
        static GLint status{};
        glGetShaderiv(id, GL_VALIDATE_STATUS, &status);
    }

    static inline RendererID CreateProgram() noexcept
    {
        const RendererID id{ glCreateProgram() };
        spdlog::debug("[OpenGL] Creating a program (id: {})", id);
        return id;
    }

    static inline void DeleteProgram(RendererID& id) noexcept
    {
        if (!DoesProgramExist(id))
        {
            spdlog::critical("[OpenGL] Invalid program id (id: {})!", id);
            return;
        }

        spdlog::debug("[OpenGL] Deleting the program (id: {})!", id);
        glDeleteProgram(id);
    }

    static inline void AttachShader(const RendererID program, const RendererID shader) noexcept
    {
        if (!DoesProgramExist(program))
        {
            spdlog::critical("[OpenGL] Got invalid program id when attaching a shader!");
        }

        glAttachShader(program, shader);
        spdlog::debug("[OpenGL] Attaching a shader (id: {}) to a program (id: {})", shader, program);
    }

    static RendererID CreateShader(const ShaderType type, const std::string_view source) noexcept
    {
        if (!Internal::IsShaderTypeValid(type))
        {
            spdlog::critical("[OpenGL] Shader creation: invalid shader type!");
            return static_cast<RendererID>(0);
        }

        if (!source.data())
        {
            spdlog::critical("[OpenGL] Shader creation: empty source!");
            return static_cast<RendererID>(0);
        }

        const RendererID id{ glCreateShader(Internal::GetGLShaderType(type)) };
        spdlog::debug("[OpenGL] Creating a shader (id: {})", id);
        const auto csource{ source.data() };
        glShaderSource(id, 1, &csource, nullptr);

        return id;
    }
}

Shader::Shader(const ShaderProps& props)
{
    for (const auto& [type, filepath] : props.Filepaths)
    {
        continue_if(filepath.empty());
        Shader::LoadFromFile(type, filepath, props.CompileAndLink);
    }

    if (props.CompileAndLink)
        Shader::Link();
}

Shader::~Shader()
{
    OpenGL::DeleteProgram(m_RendererID);
}

bool Shader::LoadFromText(const ShaderType type, const std::string_view source, bool recompile)
{
    const RendererID shader{ OpenGL::CreateShader(type, source) };
    m_Shaders[Internal::ShaderTypeIndex(type)] = shader;

    return recompile ? Shader::CompileShader(shader) : true;
}

bool Shader::LoadFromFile(const ShaderType type, const std::string_view filepath, bool recompile)
{
    const auto source{ Internal::LoadFileContent(filepath) };
    return Shader::LoadFromText(type, source, recompile);
}

bool Shader::Compile()
{
    for (std::size_t i{ 0u }; i < Shader::c_ShaderCount && m_Shaders[i]; ++i)
    {
        bool success{ Shader::CompileShader(m_Shaders[i]) };
        if (!success) return false;
    }

    return true;
}

bool Shader::Link()
{
    const RendererID program{ OpenGL::CreateProgram() };

    for (std::size_t i{ 0u }; i < Shader::c_ShaderCount && m_Shaders[i]; ++i)
        OpenGL::AttachShader(m_RendererID, m_Shaders[i]);
        
}

RENDERER_CODE_END
