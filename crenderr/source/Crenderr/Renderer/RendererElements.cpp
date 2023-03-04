#include "RendererElements.hpp"

#include <glm/ext/matrix_transform.hpp>

NAMESPACE_BEGIN(Renderer)

glm::mat4 Translation::ComposeModelMatrix() const
{
    auto model{ glm::identity<glm::mat4>() };
    model = glm::translate(model, Position);
    model = glm::rotate(model, glm::radians(Rotation.x), { 1.0f, 0.0f, 0.0f, });
    model = glm::rotate(model, glm::radians(Rotation.y), { 0.0f, 1.0f, 0.0f, });
    model = glm::rotate(model, glm::radians(Rotation.z), { 0.0f, 0.0f, 1.0f, });
    model = glm::scale(model, Scale);

    return model;
}

NAMESPACE_END(Renderer)
