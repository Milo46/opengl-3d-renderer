#pragma once

#include <spdlog/spdlog.h>

#include <functional>
#include <string>

struct CheckInstruction
{
    const std::function<bool()> Function;
    const std::string_view OnError;
};

class Checker
{
public:
    // using CheckSequence = std::vector<CheckInstruction>;
    using CheckSequence = std::initializer_list<CheckInstruction>;

public:
    Checker() = delete;

public:
    static bool PerformSequence(spdlog::level::level_enum severity, const CheckSequence& sequence);
};
