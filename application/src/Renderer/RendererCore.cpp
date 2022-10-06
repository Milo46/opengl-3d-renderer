#include "RendererCore.hpp"

RENDERER_CODE_BEGIN

RendererElement::RendererElement(const RendererID id)
    : m_RendererID{ id } {}

const RendererID RendererElement::GetHandle() const noexcept
{
    return m_RendererID;
}

RENDERER_CODE_END
