#include "Scene.hpp"

bool Scene::OnInit()
{
    return true;
}

void Scene::OnImGuiRender(ImGuiIO& io, const Timestamp& timestamp)
{
}

Scene::Scene(std::unique_ptr<Window>& window) noexcept
    : m_WindowRef{ window } {}
