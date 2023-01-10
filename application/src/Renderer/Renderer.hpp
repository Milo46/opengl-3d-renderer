#pragma once

#include "Renderer/RendererCore.hpp"
#include "Renderer/RenderCommand.hpp"

#include "Renderer/Camera/OrthographicCamera.hpp"
#include "Renderer/Camera/PerspectiveCamera.hpp"

#include <glm/glm.hpp>

RENDERER_CODE_BEGIN

class Renderer2D
{
public: // experimental (do not deploy)
    static const std::shared_ptr<class Shader>& GetFlatShader() noexcept;

    static std::size_t GetTriangleCount() noexcept;

public:
    static bool Initialize();
    static void Shutdown();

    static void BeginScene(const OrthographicCamera& camera);
    static void BeginScene(const Camera* camera);
    static void EndScene();

    static void DrawRectangle(const glm::vec3& size, const glm::vec3& position, const glm::vec3& color = glm::vec3(1.0f));
};

RENDERER_CODE_END
