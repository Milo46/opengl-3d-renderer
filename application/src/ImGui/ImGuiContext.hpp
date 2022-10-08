#pragma once

#include "Window/Window.hpp"
#include <imgui.h>

// Temporary name.
class ImGuiBuildContext
{
public:
    ImGuiBuildContext() = default;

    bool Initialize(const std::unique_ptr<Window>& window) noexcept;

    void PreRender() noexcept;
    void PostRender() noexcept;

    void Shutdown() noexcept;

private:
    ImGuiContext* m_Context{ nullptr };
    ImGuiIO* m_IO{ nullptr };
    ImGuiStyle* m_Style{ nullptr };
};
