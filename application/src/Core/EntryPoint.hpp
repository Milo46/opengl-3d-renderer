#pragma once

#include "Application/Application.hpp"
#include "Utility/Checker.hpp"

int main(int argc, char** argv)
{
    /*
        EngineAllocator::Create(Unique/Shared)<>() will take care of:
            - allocating the memory for the object,
            - informs what is being allocated (name, size, id, ...).
    */
    // auto application{ EngineAllocator::CreateUnique<Application>() };

    auto application{ std::make_unique<Application>(ApplicationProps{
        .Name       = "Application",
        .WindowSize = { 800u, 600u, },
    }) };

    if (!Checker::PerformSequence(spdlog::level::critical, {
        { [&application]() { return application.get();         }, "Failed to create the application!",     },
        { [&application]() { return application->Initialize(); }, "Failed to initialize the application!", },
    })) return EXIT_FAILURE;

    application->Run();

    return EXIT_SUCCESS;
}
