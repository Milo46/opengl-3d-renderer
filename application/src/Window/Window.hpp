#pragma once

#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <string>

#include <glm/glm.hpp>

#include "Renderer/GraphicsContext.hpp"
#include "Utility/NonCopyable.hpp"

struct GLFWwindow;
struct GLFWmonitor;

enum class WindowAction
{
    None = 0,

    ToggleFullscreen,
    Miximize,
    Iconify,
    Close,
};

class Window : public NonCopyable<Window>
{
    friend void WindowFramebufferCallback(GLFWwindow*, int, int);
    friend void WindowKeyCallback(GLFWwindow*, int, int, int, int);
    friend void WindowFocusCallback(GLFWwindow*, int);

public:
    using KeybindsContainer = std::unordered_map<WindowAction, std::unordered_set<int>>;

    static const KeybindsContainer c_DefaultKeybinds;
    // { WindowAction::ToggleFullscreen, { GLFW_KEY_F11,    }, },
    // { WindowAction::Close,            { GLFW_KEY_ESCAPE, }, },

public:
    Window() = default;
    explicit Window(const std::string_view title, const glm::ivec2& size) noexcept;
    ~Window();

    bool Initialize() noexcept;
    bool IsOpen() noexcept;

    void OnUpdate() noexcept;

    void Maximize() noexcept;
    void Iconify() noexcept;
    void Close() noexcept;

public:
    void SetKeybinds(const KeybindsContainer& keybinds) noexcept;
    void AddKeybinds(const KeybindsContainer& keybinds) noexcept;

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

    KeybindsContainer m_Keybinds{ Window::c_DefaultKeybinds };
};
