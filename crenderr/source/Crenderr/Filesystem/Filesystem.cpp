#include "Filesystem.hpp"

#include <spdlog/spdlog.h>

#include <filesystem>

std::filesystem::path PopLastElement(const std::filesystem::path& path, std::size_t count = 1u)
{
    auto strRepr{ path.string() };
    for (std::size_t i = 0u; i < count; ++i)
    {
        const auto pos{ strRepr.find_last_of("\\/") };
        if (pos == std::string::npos) return {};

        strRepr = strRepr.substr(0u, pos);
    }

    return strRepr;
}

bool Filesystem::Initialize() noexcept
{
#ifdef NDEBUG

    // Release configuration should work a little bit different than the debug.
    const auto currentDir{ std::filesystem::current_path().string() };

#else

    // Please, do not move this file, otherwise it'll break.
    const std::filesystem::path currentFilePath{ __FILE__ };
    const std::filesystem::path currentProjectPath{ PopLastElement(currentFilePath, 4u) };

    std::filesystem::current_path(currentProjectPath);
    spdlog::info("[Filesystem]: Current path: {}", currentProjectPath.string());

#endif

    return true;
}
