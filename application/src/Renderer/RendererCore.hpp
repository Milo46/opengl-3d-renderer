#pragma once

#include "Utility/NonCopyable.hpp"

#include <memory>

#define RENDERER_CODE_BEGIN namespace Renderer {
#define RENDERER_CODE_END   }

/**
 * Clean this up, please.
 */
namespace Renderer
{
    // Unfortunately I cannot include glad here ;(
    // So I have to believe that glad types are correct.
    using RendererID       = unsigned int;  // GLuint
    using RendererEnum     = unsigned int;  // GLenum
    using RendererSizei    = int;           // GLsizei
    using RendererSizeiptr = long long int; // GLsizeiptr

    using IndexType = unsigned int;

    template<typename _Ty>
    constexpr inline auto GetArithmetic(const _Ty& value) noexcept
    {
        static_assert(std::is_arithmetic_v<_Ty>);
        return value;
    }

    template<typename _Ty> constexpr inline auto c_EmptyValue{ GetArithmetic<_Ty>(0) };
    template<typename _Ty> constexpr inline auto c_InvalidValue{ static_cast<_Ty>(-1) };

    class RendererResource
    {
    public: // #1. Is virtually destructible.
        explicit RendererResource() noexcept = default;
        virtual ~RendererResource() noexcept = default;

    public: // #2. Is noncopyable.
        RendererResource(const RendererResource&) = delete;
        RendererResource& operator=(const RendererResource&) = delete;

    public: // #3. Can be bounded and unbounded.
        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

    public: // #4. First opengl calls and allocation must be done here.
        virtual bool OnInitialize() noexcept = 0;

    public: // #5. Returns the ID given by the graphics API.
        virtual RendererID GetResourceHandle() const = 0;
    };

    template<typename _DefaultProps>
    struct Creatable { using DefaultPropsType = _DefaultProps; };

    template<typename _ReturnType, typename _PropsType = typename _ReturnType::DefaultPropsType>
    inline auto Create(const _PropsType& props) noexcept -> std::shared_ptr<_ReturnType>;
}

namespace Renderer::EnumHelpers
{
    using DefaultEnumIndexType = std::size_t;

    template<typename _EnumClass, typename _IndexType = DefaultEnumIndexType>
    constexpr inline auto GetEnumClassSize() noexcept
    {
        return static_cast<_IndexType>(_EnumClass::EnumEnd);
    }

    template<typename _EnumClass, typename _IndexType = DefaultEnumIndexType>
    constexpr inline auto c_EnumClassSize{ GetEnumClassSize<_EnumClass, _IndexType>() };

    template<typename _EnumClass, typename _ReturnType>
    using EnumClassMapType = std::array<_ReturnType, c_EnumClassSize<_EnumClass>>;

    template<typename _EnumClass, typename _IndexType = DefaultEnumIndexType>
    constexpr inline auto ToIndex(_EnumClass value) noexcept
    {
        static_assert(std::is_integral_v<_IndexType>, "_IndexType has to be an integral!");
        return static_cast<_IndexType>(value) - 1u;
    }

    template<typename _EnumClass, typename _IndexType = DefaultEnumIndexType>
    constexpr inline auto ToEnumClass(_IndexType index) noexcept
    {
        static_assert(std::is_integral_v<_IndexType>, "_IndexType has to be an integral!");
        return static_cast<_EnumClass>(index + 1u);   
    }
    
    template<typename _EnumClass>
    constexpr inline auto IsEnumClassValid(_EnumClass value) noexcept
    {
        return _EnumClass::None < value && value < _EnumClass::EnumEnd;
    }

    template<typename _EnumClass, typename _ReturnType>
    constexpr inline auto MapEnumClass(
        _EnumClass value,
        const EnumClassMapType<_EnumClass, _ReturnType>& map,
        const _ReturnType& onInvalidValue = _ReturnType{}
    ) noexcept
    {
        return IsEnumClassValid(value) ? map.at(ToIndex(value)) : onInvalidValue;
    }
}

#define DECLARE_CREATABLE(_ClassName) \
    using DefaultPropsType = _ClassName##Props

#define DEFINE_DEFAULT_CREATE(_ClassName) \
    template<> \
    inline std::shared_ptr<_ClassName> Create<_ClassName, _ClassName::DefaultPropsType>(const _ClassName::DefaultPropsType& props) noexcept \
    { \
        static_assert(std::is_base_of_v<RendererResource, _ClassName>); \
        return std::make_shared<_ClassName>(props); \
    }

#define CREATE_ENUM_MAPPING(_EnumType, _ReturnType, _Name, ...) \
    constexpr std::array<_ReturnType, static_cast<std::size_t>(_EnumType::EnumEnd)> _Name{ __VA_ARGS__ }
