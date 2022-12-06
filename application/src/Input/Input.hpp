#pragma once

#include <functional>
#include <xutility>
#include <memory>

// Standard GetMousePosition() returns a new pair,
// instead we can force platform input to store it
// and make it responsible for that. Idk if it really helps with something lol.
#define INPUT_OPT_MOUSEPOS_CONSTREF

class Input
{
public:
    struct ConstructionData
    {
        std::function<Input*()> Allocator;
        std::string_view StringName;
    };

public:
    static bool Initialize(void* platformContext) noexcept;
    static void Shutdown() noexcept;

public:
    inline static auto IsKeyPressed(int keycode)        { return s_Instance->IsKeyPressedImpl(keycode); }
    inline static auto IsMouseButtonPressed(int button) { return s_Instance->IsMouseButtonPressedImpl(button); }
    inline static auto GetMousePosition()               { return s_Instance->GetMousePositionImpl(); }
    inline static auto GetMouseX()                      { return s_Instance->GetMouseXImpl(); }
    inline static auto GetMouseY()                      { return s_Instance->GetMouseYImpl(); }

protected:
    virtual bool IsKeyPressedImpl(int keycode)                    = 0;
    virtual bool IsMouseButtonPressedImpl(int button)             = 0;
    virtual float GetMouseXImpl()                                 = 0;
    virtual float GetMouseYImpl()                                 = 0;

#ifdef INPUT_OPT_MOUSEPOS_CONSTREF
    virtual const std::pair<float, float>& GetMousePositionImpl() = 0;
#else
    virtual std::pair<float, float> GetMousePositionImpl() = 0;
#endif

private:
    static std::unique_ptr<Input> s_Instance;
};
