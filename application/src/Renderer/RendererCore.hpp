#pragma once

#include <memory>

#define RENDERER_CODE_BEGIN namespace Renderer {
#define RENDERER_CODE_END   }

namespace Renderer
{
    // Unfortunately I cannot include glad here ;(
    // So I have to believe that glad types are correct.
    using RendererID       = unsigned int;  // GLuint
    using RendererEnum     = unsigned int;  // GLenum
    using RendererSizei    = int;           // GLsizei
    using RendererSizeiptr = long long int; // GLsizeiptr

    using IndexType = unsigned int;

    const RendererID c_EmptyID{ 0u };

    class RendererElement
    {
    public:
        virtual ~RendererElement() = default;
        const RendererID GetHandle() const noexcept;

    protected:
        explicit RendererElement(const RendererID id = c_EmptyID);

    protected:
        RendererID m_RendererID;
    };

    template<typename _DefaultProps>
    struct Creatable { using DefaultPropsType = _DefaultProps; };

    template<typename _ReturnType, typename _PropsType = _ReturnType::DefaultPropsType>
    inline auto Create(const _PropsType& props) noexcept -> std::shared_ptr<_ReturnType>;
}

#define DECLARE_CREATABLE(_ClassName) \
    using DefaultPropsType = _ClassName##Props

#define DEFINE_DEFAULT_CREATE(_ClassName) \
    template<> \
    inline std::shared_ptr<_ClassName> Create<_ClassName, _ClassName::DefaultPropsType>(const _ClassName::DefaultPropsType& props) noexcept \
    { \
        static_assert(std::is_base_of_v<RendererElement, _ClassName>); \
        return std::make_shared<_ClassName>(props); \
    }
