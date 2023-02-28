#include "UserApplication.hpp"
#include "UserScene.hpp"

#include <iostream>

UserApplication::UserApplication(const ApplicationProps& props) noexcept
    : Application{ props }
{
    m_ActiveScene = std::make_shared<UserScene>(Application::GetWindow());
}

UserApplication::~UserApplication()
{}

bool UserApplication::OnInitApp()
{
    if (!m_ActiveScene->OnInit())
    {
        return false;
    }

    return true;
}

void UserApplication::OnUpdate(const Timestamp& timestamp)
{
    m_ActiveScene->OnUpdate(timestamp);
}

void UserApplication::OnRender()
{
    m_ActiveScene->OnRender();
}

void UserApplication::OnImGuiRender(ImGuiIO& io)
{
    m_ActiveScene->OnImGuiRender(io, {});
}

std::unique_ptr<Application> CreateApplication() noexcept
{
    ApplicationProps props{};
    props.Name = "UserApplication";
    props.WindowSize = { 800u, 600u, };

    return std::make_unique<UserApplication>(props);
}

