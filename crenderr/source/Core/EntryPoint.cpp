#include "Application/Application.hpp"
#include "Utility/Checker.hpp"

extern std::unique_ptr<Application> CreateApplication() noexcept;

int main(int argc, char** argv)
{
    auto application{ CreateApplication() };

    if (!Checker::PerformSequence(spdlog::level::critical, {
        { [&application]() { return application.get();     }, "Failed to create the application!",     },
        { [&application]() { return application->OnInit(); }, "Failed to initialize the application!", },
    })) return EXIT_FAILURE;

    application->Run();

    return EXIT_SUCCESS;
}
