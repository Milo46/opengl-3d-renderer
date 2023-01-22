#pragma once

#include "RendererCore.hpp"

#include <unordered_map>
#include <filesystem>
#include <memory>
#include <vector>
#include <string>
#include <array>

#include <glad/glad.h>

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
    std::unordered_map<ShaderType, FileManager> Sources{};
};

class Shader : public RendererResource
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
    virtual bool OnInitialize() noexcept override;

public:
    virtual void Bind() const override;
    virtual void Unbind() const override;

public:
    inline virtual RendererID GetResourceHandle() const override { return m_RendererID; }

private:
    bool InternalLoadSource(const ShaderType type, const FileManager& source) noexcept;
    bool InternalCompileShader(const std::size_t index);

private:
    RendererID m_RendererID{ c_EmptyValue<RendererID> };
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
    std::vector<ShaderData> ShaderDataVector{};
    std::shared_ptr<const Shader> Ref;

    ShaderDataExtractor() = default;
    explicit ShaderDataExtractor(const std::shared_ptr<Shader>& shader) noexcept;
    void Extract(const std::shared_ptr<Shader>& shader) noexcept;
};

RENDERER_CODE_END

#include "Shader.inl"
