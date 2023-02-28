#pragma once

#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <string>

#include <glm/glm.hpp>

struct GLFWwindow;
struct GLFWmonitor;

namespace Renderer
{
    class GraphicsContext;
}

enum class WindowAction
{
    None = 0,

    ToggleFullscreen,
    Maximize,
    Minimize,
    Close,
};

/**
 * TODO: to complete, make the window an event emitter.
 */
enum class WindowEvent {};

class Window
{
    friend void WindowFramebufferCallback(GLFWwindow*, int, int);
    friend void WindowKeyCallback(GLFWwindow*, int, int, int, int);
    friend void WindowFocusCallback(GLFWwindow*, int);
    friend void WindowPositionCallback(GLFWwindow*, int, int);

public:
    using KeybindsContainer = std::unordered_map<WindowAction, std::unordered_set<int>>;
    static const KeybindsContainer c_DefaultKeybinds;

public:
    Window() = default;
    explicit Window(const std::string_view title, const glm::ivec2& size) noexcept;
    ~Window();

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

public:
    bool OnInit() noexcept;
    bool IsOpen();

    void OnUpdate();

    void Maximize();
    void Minimize();
    void Close();

public:
    void SetKeybinds(const KeybindsContainer& keybinds);
    void AddKeybinds(const KeybindsContainer& keybinds);

public:
    bool IsFullscreen() const;
    void SetFullscreen(bool flag);

    bool IsFocused() const;

    bool IsVSync() const;
    bool SetVSync(bool flag);

    const glm::ivec2& GetSize() const;
    const glm::ivec2& GetPosition() const;

    void SetSize(const glm::ivec2& size);
    void SetPosition(const glm::ivec2& position);

    float GetAspectRatio() const;

public:
    inline GLFWwindow* GetNativeWindow() const { return m_Window; }

private:
    GLFWwindow* m_Window{ nullptr };
    GLFWmonitor* m_Monitor{ nullptr };

    std::unique_ptr<Renderer::GraphicsContext> m_GraphicsContext;

    std::string m_Title;
    glm::ivec2 m_Size;
    glm::ivec2 m_SizeBackup;
    glm::ivec2 m_Position;
    glm::ivec2 m_PositionBackup;

    bool m_IsFullscreen{ false };
    bool m_IsFocused{ true };
    bool m_IsVSync{ true };

    KeybindsContainer m_Keybinds{ Window::c_DefaultKeybinds };
};
