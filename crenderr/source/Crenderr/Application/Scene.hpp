#pragma once

#include "ImGui/ImGuiContext.hpp"

#include "Timestamp.hpp"

class Scene
{
public:
    virtual bool OnInit();

    virtual void OnUpdate(const Timestamp& timestamp) = 0;
    virtual void OnRender() = 0;

    virtual void OnImGuiRender(ImGuiIO& io, const Timestamp& timestamp);

public:
    virtual ~Scene() = default;

protected:
    Scene(std::unique_ptr<Window>& window) noexcept;

    inline auto& GetWindow() { return m_WindowRef; }

private:
    std::unique_ptr<Window>& m_WindowRef;
};
