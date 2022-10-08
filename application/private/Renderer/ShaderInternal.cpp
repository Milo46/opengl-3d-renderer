#include "ShaderInternal.hpp"

#include <spdlog/spdlog.h>

namespace Renderer::OpenGLCommand
{
    bool SetShaderSource(const RendererID id, const std::string_view source) noexcept
    {
        const auto csource{ source.data() };
        glShaderSource(id, 1, &csource, nullptr);
        return true;
    }

    void CompileShader(const RendererID id) noexcept
    {
        glCompileShader(id);
    }

    RendererID CreateShader(const ShaderType type, const std::string_view source) noexcept
    {
        auto id{ CreateShader(type) };
        if (!SetShaderSource(id, source))
        {
            DeleteShader(id);
            return ::Renderer::c_EmptyID;
        }

        return { id };
    }

    GLint GetShaderParameter(const RendererID id, const ShaderParameter parameter) noexcept
    {
        GLint status{};
        glGetShaderiv(id, static_cast<RendererEnum>(parameter), &status);

        return status;
    }

    std::string GetShaderInfoLog(const RendererID id) noexcept
    {
        std::string infoLog{};
        const auto size{ GetShaderParameter(id, ShaderParameter::InfoLogLength) };
        infoLog.resize(size);
        glGetShaderInfoLog(id, size, nullptr, &infoLog[0u]);
        return infoLog;
    }

    std::string GetProgramInfoLog(const RendererID id) noexcept
    {
        std::string infoLog{};
        const auto size{ GetProgramParameter(id, ProgramParameter::InfoLogLength) };
        infoLog.resize(size);
        glGetProgramInfoLog(id, size, nullptr, &infoLog[0u]);
        return infoLog;
    }

    GLint GetProgramParameter(const RendererID id, const ProgramParameter parameter) noexcept
    {
        GLint status{};
        glGetProgramiv(id, static_cast<RendererEnum>(parameter), &status);

        return status;
    }
}
