#pragma once

#include "Renderer/RendererCore.hpp"
#include "Renderer/Shader.hpp"

#include <GLAD/glad.h>

RENDERER_CODE_BEGIN

namespace Internal
{
    constexpr auto c_InvalidGLShader{ static_cast<RendererEnum>(-1) };
    constexpr auto c_InvalidGLShaderString{ static_cast<const char*>("InvalidShader") };

    constexpr static inline std::size_t ShaderTypeIndex(const ShaderType type) noexcept
    {
        return static_cast<std::size_t>(type) - 1u;
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

        return IsShaderTypeValid(type) ? glShaderNames.at(ShaderTypeIndex(type)) : c_InvalidGLShader;
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

        return IsShaderTypeValid(type) ? glShaderStrings.at(ShaderTypeIndex(type)) : c_InvalidGLShaderString;
    }
}

/**
 * TODO: #1 Add OpenGL logs for all (most of the) calls!
 * TODO: #2 Add only-debug checks for correct data!
 * TODO: #3 Test if all OpenGL functions are really exception safe!
 */
namespace OpenGLCommand
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

     inline bool IsShader(const RendererID id)  noexcept { return glIsShader(id); }
     inline bool IsProgram(const RendererID id) noexcept { return glIsProgram(id); }

     inline RendererID CreateShader(const ShaderType type) noexcept { return glCreateShader(Internal::GetGLShaderType(type)); }
     inline RendererID CreateProgram() noexcept { return glCreateProgram(); }

     inline void UseProgram(const RendererID id) noexcept { glUseProgram(id); }

     inline void DeleteShader(const RendererID id) noexcept { glDeleteShader(id); }
     inline void DeleteProgram(const RendererID id) noexcept { glDeleteProgram(id); }

     inline void AttachShader(const RendererID program, const RendererID shader) noexcept
    {
        glAttachShader(program, shader);
    }

     inline void LinkProgram(const RendererID id) noexcept
    {
        glLinkProgram(id);
    }

    bool SetShaderSource(const RendererID id, const std::string_view source) noexcept;
    void CompileShader(const RendererID id) noexcept;
    RendererID CreateShader(const ShaderType type, const std::string_view source) noexcept;

    GLint GetShaderParameter(const RendererID id, const ShaderParameter parameter) noexcept;
    GLint GetProgramParameter(const RendererID id, const ProgramParameter parameter) noexcept;

    std::string GetShaderInfoLog(const RendererID id) noexcept;
    std::string GetProgramInfoLog(const RendererID id) noexcept;
}

RENDERER_CODE_END
