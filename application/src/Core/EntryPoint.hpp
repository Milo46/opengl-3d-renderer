#pragma once

#include "Application/Application.hpp"
#include "Utility/Checker.hpp"

std::unique_ptr<Application> CreateApplication() noexcept;

int main(int argc, char** argv)
{
    auto application{ CreateApplication() };

    if (!Checker::PerformSequence(spdlog::level::critical, {
        { [&application]() { return application.get();         }, "Failed to create the application!",     },
        { [&application]() { return application->Initialize(); }, "Failed to initialize the application!", },
    })) return EXIT_FAILURE;

    application->Run();

    return EXIT_SUCCESS;
}
