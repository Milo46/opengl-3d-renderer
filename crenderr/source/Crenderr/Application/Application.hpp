#pragma once

#include "Window/Window.hpp"
#include "Renderer/Backend/Framebuffer.hpp"

#include "Scene.hpp"

struct ApplicationProps
{
    std::string_view Name{};
    glm::ivec2 WindowSize{};
};

class Application
{
public:
    explicit Application(const ApplicationProps& props) noexcept;
    ~Application();

    bool OnInit();
    void Run();

protected:
    virtual bool OnInitApp();

    virtual void OnUpdate(const Timestamp& timestamp) = 0;
    virtual void OnRender() = 0;
    virtual void OnImGuiRender(ImGuiIO& io) = 0;

protected:
    inline auto& GetWindow() { return m_Window; }
    inline const auto& GetWindow() const { return m_Window; }

private:
    std::unique_ptr<Window> m_Window;
    std::unique_ptr<ImGuiBuildContext> m_ImGuiContext;
    Timestamp m_Timestamp{};
};
