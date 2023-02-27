#pragma once

#include <Application/Application.hpp>

class UserApplication : public Application
{
public:
    explicit UserApplication(const ApplicationProps& props) noexcept;
    ~UserApplication();

protected:
    virtual bool OnInitApp() override;

    virtual void OnUpdate(const Timestamp& timestamp) override;
    virtual void OnRender() override;
    virtual void OnImGuiRender(ImGuiIO& io) override;

private:
    std::shared_ptr<Scene> m_ActiveScene{};
};
