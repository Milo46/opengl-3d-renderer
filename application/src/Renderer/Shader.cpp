#include "Shader.hpp"

#include "../private/Renderer/ShaderInternal.hpp"

#include <spdlog/spdlog.h>

RENDERER_CODE_BEGIN

Shader::Shader(const ShaderProps& props) noexcept
    : RendererElement{ ::Renderer::c_EmptyID }
{
    for (const auto& [type, file] : props.Sources)
        Shader::LoadSource(type, file);
}

Shader::~Shader() noexcept
{
    OpenGLCommand::DeleteProgram(m_RendererID);
}

bool Shader::LoadSource(const ShaderType type, const std::string& source) noexcept
{
    return Shader::InternalLoadSource(type, FileManagerHelper::CreateFromContent(source));
}

bool Shader::LoadSource(const ShaderType type, FileManager& source) noexcept
{
    if (!source.IsLoaded()) source.Load();

    return Shader::InternalLoadSource(type, source);
}

bool Shader::LoadSource(const ShaderType type, const FileManager& source) noexcept
{
    if (!source.IsLoaded())
    {
        spdlog::error("[Shader::LoadSource()] Not loaded source cannot be passed as a const& argument!");
        return false;
    }

    return Shader::InternalLoadSource(type, source);
}

bool Shader::Compile() noexcept
{
    bool retval{ true };
    for (std::size_t i = 0u; i < m_Handles.size() && m_Handles[i] != c_EmptyID; ++i)
    {
        const bool success{ Shader::InternalCompileShader(i) };
        if (!success) retval = false;
    }

    return retval;
}

bool Shader::Link() noexcept
{
    RendererID program{ OpenGLCommand::CreateProgram() };

    for (const auto& it : m_Handles)
    {
        if (it == c_EmptyID) continue;
        OpenGLCommand::AttachShader(program, it);
    }

    OpenGLCommand::LinkProgram(program);
    const auto status{ OpenGLCommand::GetProgramParameter(program, OpenGLCommand::ProgramParameter::LinkStatus) };
    if (!status)
    // if (const auto status{ OpenGLCommand::GetProgramParameter(program, OpenGLCommand::ProgramParameter::LinkStatus) })
    {
        const auto infoLog{ OpenGLCommand::GetProgramInfoLog(program) };
        spdlog::error("[Shader::Link()] Failed to link the program (id: {}): {}", program, infoLog);
        return false;
    }

    if (m_RendererID != c_EmptyID) OpenGLCommand::DeleteProgram(m_RendererID);

    m_RendererID = program;
    return true;
}

void Shader::Bind() const
{
    OpenGLCommand::UseProgram(m_RendererID);
}

void Shader::Unbind() const
{
    OpenGLCommand::UseProgram(0u);
}

bool Shader::InternalLoadSource(const ShaderType type, const FileManager& source) noexcept
{
    // Wait for complete shader class in order to test this case.
    // if (source.GetContent() == FileManager::c_NoContent)
    // {
    //     spdlog::error("[Shader::LoadSource()] In order to load the shader\'s source, it has to have content!");
    //     return false;
    // }

    m_Handles [Internal::ShaderTypeIndex(type)] = OpenGLCommand::CreateShader(type, source.GetContent());
    m_Sources [Internal::ShaderTypeIndex(type)] = source;
    m_Compiled[Internal::ShaderTypeIndex(type)] = false;

    return true;
}

bool Shader::InternalCompileShader(const std::size_t index)
{
    if (m_Compiled[index]) return true;

    const auto id{ m_Handles[index] };
    OpenGLCommand::CompileShader(id);

    const auto status{ OpenGLCommand::GetShaderParameter(id, OpenGLCommand::ShaderParameter::CompileStatus) };
    if (!status)
    {
        const auto infoLog{ OpenGLCommand::GetShaderInfoLog(id) };
        spdlog::error("[Shader::CompileShader()] Failed to compile a shader (id: {}): {}", id, infoLog);
        return false;
    }

    return true;
}

ShaderDataExtractor::ShaderDataExtractor(const std::shared_ptr<Shader>& shader) noexcept
    : Ref{ shader }
{
    for (std::size_t i{ 0u }; i < shader->m_Handles.size() && shader->m_Handles[i] != c_EmptyID; ++i)
    {
        ShaderData.push_back({
            .ID     = shader->m_Handles[i],
            .Type   = Internal::GetShaderTypeString(static_cast<ShaderType>(i + 1u)),
            .Source = shader->m_Sources[i],
        });
    }
}

RENDERER_CODE_END
