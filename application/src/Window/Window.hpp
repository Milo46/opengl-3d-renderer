#pragma once

#include <memory>
#include <string>

#include <glm/glm.hpp>

#include "Renderer/GraphicsContext.hpp"
#include "Utility/NonCopyable.hpp"

struct GLFWwindow;
struct GLFWmonitor;

class Window : public NonCopyable<Window>
{
    friend void WindowFramebufferCallback(GLFWwindow*, int, int);
    friend void WindowKeyCallback(GLFWwindow*, int, int, int, int);
    friend void WindowFocusCallback(GLFWwindow*, int);

public:
    enum class Action
    {
        None = 0,

        ToggleFullscreen,
        Close,
    };

public:
    Window() = default;
    explicit Window(const std::string_view title, const glm::ivec2& size) noexcept;
    ~Window();

    bool Initialize() noexcept;
    bool IsOpen() noexcept;

    void OnUpdate() noexcept;

public:
    bool IsFullscreen() const;
    void SetFullscreen(bool flag);

    bool IsFocused() const;

    bool IsVSync() const;
    bool SetVSync(const bool flag);

    const glm::ivec2& GetSize() const;
    const glm::ivec2& GetPosition() const;

    void SetSize(const glm::ivec2& size);
    void SetPosition(const glm::ivec2& position);

    float GetAspectRatio() const;

public:
    inline GLFWwindow* GetNativeWindow() const noexcept { return m_Window; }

private:
    GLFWwindow* m_Window{ nullptr };
    GLFWmonitor* m_Monitor{ nullptr };

    std::unique_ptr<Renderer::GraphicsContext> m_GraphicsContext;

    std::string m_Title;
    glm::ivec2 m_Size;
    glm::ivec2 m_SizeBackup;
    glm::ivec2 m_Position;

    bool m_IsFullscreen{ false };
    bool m_IsFocused{ true };
    bool m_IsVSync{ true };
};
