#pragma once

#include <Crenderr/Application/Scene.hpp>
#include <Crenderr/Renderer/Renderer.hpp>

#include <Crenderr/Renderer/Camera/Projections/Perspective.hpp>
#include <Crenderr/Renderer/Camera/Projections/Orthographic.hpp>
#include <Crenderr/Renderer/Camera/Controllers/CameraController.hpp>

#include <map>

class UserScene : public Scene
{
public:
    explicit UserScene(std::unique_ptr<Window>& windowRef);

public:
    virtual bool OnInit() override;

    virtual void OnUpdate(const Timestamp& timestamp) override;
    virtual void OnRender() override;
    virtual void OnImGuiRender(ImGuiIO& io, const Timestamp& timestamp) override;

private:
    void RenderSingleViewport();
    void RenderDoubleViewport();

    void RenderViewport();

private:
    std::unique_ptr<Renderer::Renderer3DInstance> m_RendererContext;
    bool m_RenderDoubleViewport{ false };

    Renderer::CameraOrbitController m_OrbitController{};
    Renderer::Camera& m_Camera{ m_OrbitController.CameraHandle };

    // Renderer::Camera m_Camera{};
    // glm::vec3 m_CameraFocusPoint{ glm::vec3(0.0f) };
    // float m_CameraArmLength{ 2.0f };

    std::shared_ptr<Renderer::VertexArray> m_Model{};
    std::shared_ptr<Renderer::Texture2D> m_DiffuseMap{};
    std::shared_ptr<Renderer::Texture2D> m_SpecularMap{};
    std::shared_ptr<Renderer::Texture2D> m_EmissionMap{};

    std::shared_ptr<Renderer::VertexArray> m_Grid{};

    // bool m_IsMouseCaptured{ false };
    // bool m_IsFirstCaptureFrame{ false };
    // glm::vec2 m_PrevFrameCursorPos{};
    // glm::vec2 m_CurrFrameCursorPos{};

    // glm::vec3 m_ControllerAngles{ 90.0f, 0.0f, 0.0f, };
    // float m_MouseSensitivity{ 0.07f };
};
