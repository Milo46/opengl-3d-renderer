#include "Checker.hpp"

bool Checker::PerformSequence(spdlog::level::level_enum severity, const CheckSequence& sequence)
{
    for (const auto& [check, onError] : sequence)
    {
        if (check()) continue;
        spdlog::log(severity, onError);
        return false;
    }
    return true;
}
