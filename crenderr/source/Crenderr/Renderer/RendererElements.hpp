#pragma once

#include "RendererCore.hpp"

#include "Renderer/Backend/Buffers.hpp"

#include <glm/glm.hpp>

NAMESPACE_BEGIN(Renderer)

struct Vertex3D
{
    glm::vec3 Position{};
    glm::vec3 Normal{};
    glm::vec2 Texcoord{};

    inline static const BufferLayout c_Layout{
        { LayoutDataType::Float3, "a_Position", },
        { LayoutDataType::Float3, "a_Normal",   },
        { LayoutDataType::Float2, "a_Texcoord", },
    };
};

struct Translation
{
    glm::vec3 Scale    = { glm::vec3(1.0f) };
    glm::vec3 Position = { glm::vec3(0.0f) };
    glm::vec3 Rotation = { glm::vec3(0.0f) };

    glm::mat4 ComposeModelMatrix() const;
};

NAMESPACE_END(Renderer)
