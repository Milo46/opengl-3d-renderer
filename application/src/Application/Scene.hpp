#pragma once

#include "Application.hpp"

class UserScene : public Scene
{
public:
    explicit UserScene(std::unique_ptr<Window>& windowRef);

public:
    virtual bool OnInitialization() override;

    virtual void OnUpdate(const Timestamp& timestamp) noexcept override;
    virtual void OnRender() noexcept override;
    virtual void OnImGuiRender(ImGuiIO& io, const Timestamp& timestamp) noexcept override;

public:
    std::unique_ptr<Renderer::Renderer3DInstance> m_RendererContext;

    Renderer::PerspectiveCamera m_Camera{};
    float m_CameraArmLength{ 2.0f };

    std::shared_ptr<Renderer::VertexArray> m_Model{};
    std::shared_ptr<Renderer::Texture2D> m_DiffuseMap{};
    std::shared_ptr<Renderer::Texture2D> m_SpecularMap{};
    std::shared_ptr<Renderer::Texture2D> m_EmissionMap{};
};
