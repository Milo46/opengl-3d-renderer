#include "RendererCore.hpp"

RENDERER_CODE_BEGIN

const RendererID RendererElement::GetHandle() const noexcept
{
    return m_RendererID;
}

RENDERER_CODE_END
