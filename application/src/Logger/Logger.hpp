#pragma once

class Logger
{
public:
    Logger() = delete;

    static bool Initialize() noexcept;
    static void Shutdown() noexcept;
};
