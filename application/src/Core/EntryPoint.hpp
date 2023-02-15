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

// #include <iostream>
// #include <vector>
// #include <list>
// #include <map>

// struct IObserver
// {
//     virtual ~IObserver() = default;
//     virtual void OnEvent(const std::string& message) = 0;
// };

// struct ISubject
// {
//     virtual ~ISubject() = default;
//     virtual void Attach(IObserver* observer) = 0;
//     virtual void Detach(IObserver* observer) = 0;
//     virtual void DispatchEvent() = 0;
// };

// class Subject : public ISubject
// {
// public:
//     virtual void Attach(IObserver* observer) override
//     {
//         m_Observers.push_back(observer);
//     }

//     virtual void Detach(IObserver* observer) override
//     {
//         m_Observers.remove(observer);
//     }

//     virtual void DispatchEvent() override
//     {
//         for (auto& observer : m_Observers)
//             observer->OnEvent("Woah!");
//     }

// private:
//     std::list<IObserver*> m_Observers{};
// };

// class Observer : public IObserver
// {
// public:
//     Observer(Subject& subject)
//         : m_Subject{ subject }
//     {
//         m_Subject.Attach(this);
//     }
    
//     virtual void OnEvent(const std::string& message) override
//     {
//         std::cout << "Received a message: " << message << '\n';
//     }

// private:
//     Subject& m_Subject;
// };

// int main()
// {
//     Subject subject{};

//     Observer observer1{ subject };
//     Observer observer2{ subject };

//     subject.DispatchEvent();

//     return EXIT_SUCCESS;
// }
