#pragma once

#include "Renderer/RendererCore.hpp"
#include "Projection.hpp"

NAMESPACE_BEGIN(Renderer)

class Camera
{
public:
    std::shared_ptr<Projection> ProjectionHandle{ nullptr };
    glm::vec3 Position{ glm::vec3(0.0f) };
    glm::vec3 Rotation{ glm::vec3(0.0f) };

public:
    Camera() = default;
    Camera(std::shared_ptr<Projection> projection);

    glm::mat4 CalculateViewMatrix() const;
    glm::mat4 CalculateProjectionMatrix() const;

public:
    template<typename _Projection>
    inline std::shared_ptr<_Projection> GetProjection() const
    {
        static_assert(std::is_base_of_v<Projection, _Projection>);
        return std::dynamic_pointer_cast<_Projection>(ProjectionHandle);
    }
};

NAMESPACE_END(Renderer)
