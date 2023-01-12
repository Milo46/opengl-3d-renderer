#pragma once

#include "RendererCore.hpp"
#include "Bindable.hpp"

#include <unordered_map>
#include <xtr1common>
#include <filesystem>
#include <memory>
#include <string>
#include <array>

#include <GLAD/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Utility/FileManager.hpp"
#include "Utility/NonCopyable.hpp"

RENDERER_CODE_BEGIN

enum class ShaderType
{
    None = 0,
    Vertex = 1, Fragment, Geometry, Compute, TessControl, Evaluation,
    EnumEnd,
};

struct ShaderProps
{
    const std::unordered_map<ShaderType, FileManager> Sources{};
};

class Shader
    : public NonCopyable<Shader>,
      public RendererElement,
      public Bindable
{
public:
    DECLARE_CREATABLE(Shader);

    friend struct ShaderDataExtractor;

public:
    static constexpr auto c_ShaderCount{ EnumHelpers::GetEnumClassSize<ShaderType>() };

public:
    explicit Shader(const ShaderProps& props) noexcept;
    ~Shader() noexcept;

    bool LoadSource(const ShaderType type, const std::string& source) noexcept;
    bool LoadSource(const ShaderType type, FileManager& source) noexcept;
    bool LoadSource(const ShaderType type, const FileManager& source) noexcept;

    bool Compile() noexcept;
    bool Link() noexcept;

public:
    template<typename _Ty>
    inline void SetUniform(const std::string_view, const _Ty&) noexcept;

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

DEFINE_DEFAULT_CREATE(Shader);

struct ShaderData
{
    const RendererID ID;
    const std::string Type;
    const FileManager Source;
};

struct ShaderDataExtractor
{
    std::vector<ShaderData> ShaderData{};
    std::shared_ptr<const Shader> Ref;

    ShaderDataExtractor() = default;
    explicit ShaderDataExtractor(const std::shared_ptr<Shader>& shader) noexcept;
    void Extract(const std::shared_ptr<Shader>& shader) noexcept;
};

RENDERER_CODE_END

#include "Shader.inl"
