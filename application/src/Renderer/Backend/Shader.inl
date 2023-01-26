#pragma once
#include "Shader.hpp"

NAMESPACE_BEGIN(Renderer)

template<>
inline void Shader::SetUniform<float>(const std::string_view name, const float& value) noexcept
{
    const auto location{ glGetUniformLocation(m_RendererID, name.data()) };
    glUniform1f(location, value);
}

template<>
inline void Shader::SetUniform<glm::vec3>(const std::string_view name, const glm::vec3& value) noexcept
{
    const auto location{ glGetUniformLocation(m_RendererID, name.data()) };
    glUniform3f(location, value.x, value.y, value.z);
}

template<>
inline void Shader::SetUniform<glm::mat4>(const std::string_view name, const glm::mat4& value) noexcept
{
    const auto location{ glGetUniformLocation(m_RendererID, name.data()) };
    glUniformMatrix4fv(location, 1u, GL_FALSE, glm::value_ptr(value));
}

NAMESPACE_END(Renderer)
