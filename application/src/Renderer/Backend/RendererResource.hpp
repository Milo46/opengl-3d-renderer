#pragma once

#include "Renderer/RendererCore.hpp"

NAMESPACE_BEGIN(Renderer)

template<typename _Props>
class RendererResource
{
public:
    using PropsType = _Props;

public: // #1. Is virtually destructible.
    explicit RendererResource() noexcept = default;
    virtual ~RendererResource() noexcept = default;

public: // #2. Is noncopyable.
    RendererResource(const RendererResource&) = delete;
    RendererResource& operator=(const RendererResource&) = delete;

public: // #3. Can be bounded and unbounded.
    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

public: // #4. First opengl calls (in the object's lifetime) and allocation must be done here.
    virtual bool OnInitialize() noexcept = 0;

public: // #5. Returns the ID given by the graphics API.
    virtual RendererID GetResourceHandle() const = 0;
};

template<typename _Ty, typename _Props = typename _Ty::PropsType>
inline std::shared_ptr<_Ty> AllocateResource(const _Props& props) noexcept
{
    static_assert(std::is_base_of_v<RendererResource<typename _Ty::PropsType>, _Ty>);
    return std::make_shared<_Ty>(props);
}

NAMESPACE_END(Renderer)
