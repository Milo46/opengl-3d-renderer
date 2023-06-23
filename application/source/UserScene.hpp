#pragma once

#include <Crenderr/Application/Scene.hpp>

#include <Crenderr/Renderer/Renderer.hpp>

#include <Crenderr/Renderer/Camera/Projections/Perspective.hpp>
#include <Crenderr/Renderer/Camera/Projections/Orthographic.hpp>
#include <Crenderr/Renderer/Camera/Controllers/CameraController.hpp>

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
    void RenderViewport();

    void RenderSingleViewport();
    void RenderDoubleViewport();

private:
    std::unique_ptr<Renderer::Renderer3DInstance> m_RendererContext;
    bool m_RenderDoubleViewport{ false };

    Renderer::CameraOrbitController m_OrbitController{};
    Renderer::Camera& m_Camera{ m_OrbitController.CameraHandle };

    std::shared_ptr<Renderer::VertexArray> m_Model{};
    std::shared_ptr<Renderer::Texture2D> m_DiffuseMap{};
    std::shared_ptr<Renderer::Texture2D> m_SpecularMap{};
    std::shared_ptr<Renderer::Texture2D> m_EmissionMap{};

    std::shared_ptr<Renderer::VertexArray> m_Grid{};
};
