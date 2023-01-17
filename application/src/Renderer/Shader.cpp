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

namespace OpenGLCommand
{
    inline bool IsShader(RendererID id) noexcept  { return glIsShader(id);  }
    inline bool IsProgram(RendererID id) noexcept { return glIsProgram(id); }

    inline RendererID CreateShader(ShaderType type, const std::string& source) noexcept
    {
        RendererID id{ glCreateShader(EnumHelpers::MapEnumClass(type, Internal::c_ShaderGLType)) };
        ON_DEBUG([&]() {
            spdlog::debug("[OpenGL] Creating a shader (ID: {}, Name: {})...",
                id, EnumHelpers::MapEnumClass(type, Internal::c_ShaderStringName));
        });

        const auto csource{ source.c_str() };
        glShaderSource(id, 1, &csource, nullptr);
        ON_DEBUG([&]() {
            spdlog::debug("[OpenGL] Setting shader\'s (ID: {}, Name: {}) source...",
                id, EnumHelpers::MapEnumClass(type, Internal::c_ShaderStringName));
        });

        return id;
    }

    inline RendererID CreateProgram() noexcept
    {
        RendererID id{ glCreateProgram() };
        ON_DEBUG([&]() {
            spdlog::debug("[OpenGL] Creating a program (ID: {})...", id);
        });

        return id;
    }

    inline void DeleteProgram(RendererID& id) noexcept
    {
        if (id == c_EmptyValue<RendererID>) return;

        ON_DEBUG([&]() {
            spdlog::debug("[OpenGL] Deleting a program (ID: {})...", id);
            spdlog::debug("[OpenGL] - Checking if provided ID is a program...");
            if (!IsProgram(id))
            {
                spdlog::critical("[OpenGL](DeleteProgram) Provided ID (= {}) is not a program!", id);
                std::abort();
            }
        });

        return glDeleteProgram(id);
    }

    inline void UseProgram(RendererID id) noexcept
    {
        return glUseProgram(id);
    }

    inline std::optional<std::string> CompileShader(RendererID id) noexcept
    {
        ON_DEBUG([&]() {
            spdlog::debug("[OpenGL] Compiling a shader (ID: {})...", id);
            spdlog::debug("[OpenGL] - Checking if provided ID is a shader...");
            if (!IsShader(id))
            {
                spdlog::critical("[OpenGL](CompileShader) Provided ID (= {}) is not a shader", id);
            }
        });

        glCompileShader(id);

        GLint compileStatus{};
        glGetShaderiv(id, GL_COMPILE_STATUS, &compileStatus);
        if (!compileStatus)
        {
            std::string infoLog{};
            GLint infoLogSize{};
            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &infoLogSize);
            infoLog.resize(infoLogSize);
            glGetShaderInfoLog(id, infoLogSize, nullptr, &infoLog[0u]);

            return std::make_optional(infoLog);
        }

        return std::nullopt;
    }

    inline std::optional<std::string> LinkProgram(RendererID id) noexcept
    {
        ON_DEBUG([&]() {
            spdlog::debug("[OpenGL] Linking a program (ID: {})...", id);
            spdlog::debug("[OpenGL] - Checking if provided ID is a program...");
            if (!IsProgram(id))
            {
                spdlog::critical("[OpenGL](LinkProgram) Provided ID (= {}) is not a program!", id);
                std::abort();
            }
        });

        glLinkProgram(id);

        GLint linkStatus{};
        glGetProgramiv(id, GL_LINK_STATUS, &linkStatus);
        if (!linkStatus)
        {
            std::string infoLog{};
            GLint infoLogSize{};
            glGetProgramiv(id, GL_INFO_LOG_LENGTH, &infoLogSize);
            infoLog.resize(infoLogSize);
            glGetProgramInfoLog(id, infoLogSize, nullptr, &infoLog[0u]);

            return std::make_optional(infoLog);
        }

        return std::nullopt;
    }

    inline void AttachShader(RendererID program, RendererID shader) noexcept
    {
        ON_DEBUG([&]() {
            spdlog::debug("[OpenGL] Attaching a shader (ID: {}) to the program (ID: {})!", shader, program);
            spdlog::debug("[OpenGL] - Checking if provided ID is a shader...");
            if (!IsShader(shader))
            {
                spdlog::critical("[OpenGL](AttachShader) Provided ID (= {}) is not a shader!", shader);
                std::abort();
            }
            spdlog::debug("[OpenGL] - Checking if provided ID is a program...");
            if (!IsProgram(program))
            {
                spdlog::critical("[OpenGL](AttachShader) Provided ID (= {}) is not a program!", program);
                std::abort();
            }
        });

        return glAttachShader(program, shader);
    }
}

Shader::Shader(const ShaderProps& props) noexcept
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
    for (std::size_t i = 0u; i < m_Handles.size() && m_Handles[i] != c_EmptyValue<RendererID>; ++i)
    {
        const bool success{ Shader::InternalCompileShader(i) };
        if (!success) retval = false;
    }

    return retval;
}

bool Shader::Link() noexcept
{
    auto program{ OpenGLCommand::CreateProgram() };

    for (const auto& it : m_Handles)
    {
        if (it == c_EmptyValue<RendererID>) continue;
        OpenGLCommand::AttachShader(program, it);
    }
    
    if (const auto infoLog{ OpenGLCommand::LinkProgram(program) })
    {
        spdlog::error("Failed to link the shader program (ID: {})!\nOpenGL Info Log: {}", program, infoLog.value());
        return false;
    }

    if (m_RendererID != c_EmptyValue<RendererID>)
        OpenGLCommand::DeleteProgram(m_RendererID);

    m_RendererID = program;
    return true;
}

void Shader::Bind() const
{
    OpenGLCommand::UseProgram(m_RendererID);
}

void Shader::Unbind() const
{
    OpenGLCommand::UseProgram(c_EmptyValue<RendererID>);
}

bool Shader::InternalLoadSource(const ShaderType type, const FileManager& source) noexcept
{
    // Wait for complete shader class in order to test this case.
    // if (source.GetContent() == FileManager::c_NoContent)
    // {
    //     spdlog::error("[Shader::LoadSource()] In order to load the shader\'s source, it has to have content!");
    //     return false;
    // }

    m_Handles [EnumHelpers::ToIndex(type)] = OpenGLCommand::CreateShader(type, source.GetContent());
    m_Sources [EnumHelpers::ToIndex(type)] = source;
    m_Compiled[EnumHelpers::ToIndex(type)] = false;

    return true;
}

bool Shader::InternalCompileShader(const std::size_t index)
{
    if (m_Compiled[index]) return true;

    const auto id{ m_Handles[index] };
    if (const auto infoLog{ OpenGLCommand::CompileShader(id) })
    {
        spdlog::error("Failed to compile a shader (ID: {}, Name: {})!\nOpenGL Info Log: {}",
            id, EnumHelpers::MapEnumClass(EnumHelpers::ToEnumClass<ShaderType>(index), Internal::c_ShaderStringName), infoLog.value());
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
