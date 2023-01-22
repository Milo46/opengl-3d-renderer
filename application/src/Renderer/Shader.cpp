#include "Shader.hpp"

#include <spdlog/spdlog.h>
#include <optional>

#ifdef NDEBUG
#   define ON_DEBUG(_Func)
#else
#   define ON_DEBUG(_Func) _Func()
#endif

RENDERER_CODE_BEGIN

namespace Internal
{
    CREATE_ENUM_MAPPING(ShaderType, RendererEnum, c_ShaderGLType,
        GL_VERTEX_SHADER,          // <- ShaderType::Vertex
        GL_FRAGMENT_SHADER,        // <- ShaderType::Fragment
        GL_GEOMETRY_SHADER,        // <- ShaderType::Geometry
        GL_COMPUTE_SHADER,         // <- ShaderType::Compute
        GL_TESS_CONTROL_SHADER,    // <- ShaderType::TessControl
        GL_TESS_EVALUATION_SHADER, // <- ShaderType::Evaluation
    );

    CREATE_ENUM_MAPPING(ShaderType, const char*, c_ShaderStringName,
        "VertexShader",         // <- ShaderType::Vertex
        "FragmentShader",       // <- ShaderType::Fragment
        "GeometryShader",       // <- ShaderType::Geometry
        "ComputeShader",        // <- ShaderType::Compute
        "TessControlShader",    // <- ShaderType::TessControl
        "TessEvaluationShader", // <- ShaderType::Evaluation
    );
}

Shader::Shader(const ShaderProps& props) noexcept
{
    for (const auto& [type, file] : props.Sources)
        Shader::LoadSource(type, file);
}

Shader::~Shader() noexcept
{
    if (m_RendererID != c_EmptyValue<RendererID>)
        glDeleteProgram(m_RendererID);
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

bool Shader::OnInitialize() noexcept
{
    if (m_RendererID != c_EmptyValue<RendererID>)
        glDeleteProgram(m_RendererID);

    //?...

    return true;
}

bool Shader::Compile() noexcept
{
    bool retval{ true };
    for (std::size_t i = 0u; i < m_Handles.size() && m_Handles[i] != c_EmptyValue<RendererID>; ++i)
    {
        const bool success{ Shader::InternalCompileShader(i) };
        if (!success) retval = false;
    }

    return retval;
}

bool Shader::Link() noexcept
{
    auto program{ glCreateProgram() };

    for (const auto& it : m_Handles)
    {
        if (it == c_EmptyValue<RendererID>) continue;
        glAttachShader(program, it);
    }

    glLinkProgram(program);

    GLint linkStatus{};
    glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
    if (!linkStatus)
    {
        GLint infoLogSize{};
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogSize);

        std::string infoLog(infoLogSize, ' ');
        glGetProgramInfoLog(program, infoLogSize, nullptr, &infoLog[0u]);

        spdlog::error("Failed to link the shader program (ID: {})!\nOpenGL Info Log: {}", program, infoLog);
        return false;
    }

    if (m_RendererID != c_EmptyValue<RendererID>)
        glDeleteProgram(m_RendererID);

    m_RendererID = program;
    return true;
}

void Shader::Bind() const
{
    glUseProgram(m_RendererID);
}

void Shader::Unbind() const
{
    glUseProgram(c_EmptyValue<RendererID>);
}

bool Shader::InternalLoadSource(const ShaderType type, const FileManager& source) noexcept
{
    // Wait for complete shader class in order to test this case.
    // if (source.GetContent() == FileManager::c_NoContent)
    // {
    //     spdlog::error("[Shader::LoadSource()] In order to load the shader\'s source, it has to have content!");
    //     return false;
    // }

    RendererID id{ glCreateShader(EnumHelpers::MapEnumClass(type, Internal::c_ShaderGLType)) };

    const auto content{ source.GetContent() };
    const auto csource{ content.c_str() };
    glShaderSource(id, 1, &csource, nullptr);

    m_Handles [EnumHelpers::ToIndex(type)] = id;
    m_Sources [EnumHelpers::ToIndex(type)] = source;
    m_Compiled[EnumHelpers::ToIndex(type)] = false;

    return true;
}

bool Shader::InternalCompileShader(const std::size_t index)
{
    if (m_Compiled[index]) return true;

    const auto id{ m_Handles[index] };
    glCompileShader(id);

    GLint compileStatus{};
    glGetShaderiv(id, GL_COMPILE_STATUS, &compileStatus);
    if (!compileStatus)
    {
        GLint infoLogSize{};
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &infoLogSize);

        std::string infoLog(infoLogSize, ' ');
        glGetShaderInfoLog(id, infoLogSize, nullptr, &infoLog[0u]);

        spdlog::error("Failed to compile a shader (ID: {}, Name: {})!\nOpenGL Info Log: {}",
            id, EnumHelpers::MapEnumClass(EnumHelpers::ToEnumClass<ShaderType>(index), Internal::c_ShaderStringName), infoLog);
        return false;
    }

    return true;
}

ShaderDataExtractor::ShaderDataExtractor(const std::shared_ptr<Shader>& shader) noexcept
{
    ShaderDataExtractor::Extract(shader);
}

void ShaderDataExtractor::Extract(const std::shared_ptr<Shader>& shader) noexcept
{
    Ref = shader;
    ShaderDataVector.clear();
    for (std::size_t i{ 0u }; i < shader->m_Handles.size() && shader->m_Handles[i] != c_EmptyValue<RendererID>; ++i)
    {
        const auto shaderType{ EnumHelpers::ToEnumClass<ShaderType>(i) };
        const auto name{ EnumHelpers::MapEnumClass<ShaderType, const char*>(shaderType, Internal::c_ShaderStringName, "") };

        ShaderDataVector.push_back({
            .ID     = shader->m_Handles[i],
            .Type   = name,
            .Source = shader->m_Sources[i],
        });
    }
}

RENDERER_CODE_END
