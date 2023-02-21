#pragma once

#include "Renderer/RendererCore.hpp"

NAMESPACE_BEGIN(Renderer)

template<typename _Props>
class RendererResource
{
public:
    // #1. Is defaultly constructed from a specified data structure.
    using PropsType = _Props;

public:
    // #2. Is virtually destructible.
    virtual ~RendererResource() noexcept = default;

    // #3. Is noncopyable.
    RendererResource(const RendererResource&) = delete;
    RendererResource& operator=(const RendererResource&) = delete;

protected:
    // #4. Protected constructible.
    RendererResource() noexcept = default;

public:
    // #5. Can be bounded and unbounded.
    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    // #6. First opengl calls (in the object's lifetime) and allocation must be done here.
    virtual bool OnInitialize() noexcept = 0;

    // #7. Returns the ID given by the graphics API.
    virtual RendererID GetResourceHandle() const = 0;
};

// TODO: force this method as the only one to allocated renderer's resources, creational pattern
template<typename _Ty, typename _Props = typename _Ty::PropsType>
inline std::shared_ptr<_Ty> AllocateResource(const _Props& props) noexcept
{
    static_assert(std::is_base_of_v<RendererResource<_Props>, _Ty>);
    return std::make_shared<_Ty>(props);
}

NAMESPACE_END(Renderer)
