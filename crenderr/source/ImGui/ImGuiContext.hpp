#pragma once

#include "Window/Window.hpp"
#include <imgui.h>

class ImGuiBuildContext
{
public:
    ImGuiBuildContext() = default;

    bool Initialize(const std::unique_ptr<Window>& window) noexcept;

    void PreRender() noexcept;
    void PostRender() noexcept;

    void Shutdown() noexcept;

public:
    inline ImGuiIO& GetIO() noexcept { return *m_IO; }
    inline const ImGuiIO& GetIO() const noexcept { return *m_IO; }

private:
    ImGuiContext* m_Context{ nullptr };
    ImGuiIO* m_IO{ nullptr };
    ImGuiStyle* m_Style{ nullptr };
};
