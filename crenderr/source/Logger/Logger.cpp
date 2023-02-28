#include "Logger.hpp"

#include <spdlog/spdlog.h>

#ifdef NDEBUG
    constexpr static bool c_IsDebug{ false };
#else
    constexpr static bool c_IsDebug{ true };
#endif

bool Logger::Initialize() noexcept
{
    if constexpr (c_IsDebug)
        spdlog::set_level(spdlog::level::debug);
        
    return true;
}

void Logger::Shutdown() noexcept
{
}
