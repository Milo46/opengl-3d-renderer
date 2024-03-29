#pragma once

#include <Crenderr/Application/Scene.hpp>
#include <Crenderr/Renderer/Renderer.hpp>

class UserScene : public Scene
{
public:
    explicit UserScene(std::unique_ptr<Window>& windowRef);

public:
    virtual bool OnInit() override;

    virtual void OnUpdate(const Timestamp& timestamp) override;
    virtual void OnRender() override;
    virtual void OnImGuiRender(ImGuiIO& io, const Timestamp& timestamp) override;

public:
    std::unique_ptr<Renderer::Renderer3DInstance> m_RendererContext;

    Renderer::PerspectiveCamera m_Camera{};
    float m_CameraArmLength{ 2.0f };

    std::shared_ptr<Renderer::VertexArray> m_Model{};
    std::shared_ptr<Renderer::Texture2D> m_DiffuseMap{};
    std::shared_ptr<Renderer::Texture2D> m_SpecularMap{};
    std::shared_ptr<Renderer::Texture2D> m_EmissionMap{};

    bool m_IsMouseCaptured{ false };
    bool m_IsFirstCaptureFrame{ false };
    glm::vec2 m_PrevFrameCursorPos{};
    glm::vec2 m_CurrFrameCursorPos{};
};
