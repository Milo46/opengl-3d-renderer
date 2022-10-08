#pragma once

#include "Application/Application.hpp"

#include <spdlog/spdlog.h>

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

    if (!application.get())
    {
        spdlog::critical("Failed to create the application!");
        return EXIT_FAILURE;
    }

    if (!application->Initialize())
    {
        spdlog::critical("Failed to initialize the application!");
        return EXIT_FAILURE;
    }

    application->Run();

    // /* Overengineering? Yes, replace ordinary ifs with bullshit code, mmmm. (Maybe I'll need that later). */
    // const Checking::CheckSequence checkSequence{ {
    //     { [](&application) { return !application.get();         }, "Failed to create the application!",     spdlog::level::critical, },
    //     { [](&application) { return !application->Initialize(); }, "Failed to initialize the application!", spdlog::level::critical, },
    // } };
    // const auto Checking::Checker checker{};
    // checker.PerformSequence(checkSequence);

    return EXIT_SUCCESS;
}
