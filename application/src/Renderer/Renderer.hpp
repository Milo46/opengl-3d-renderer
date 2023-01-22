#pragma once

#include "Renderer/RendererCore.hpp"
#include "Renderer/RenderCommand.hpp"

#include "Renderer/Camera/OrthographicCamera.hpp"
#include "Renderer/Camera/PerspectiveCamera.hpp"

#include <glm/glm.hpp>

RENDERER_CODE_BEGIN

// struct PlaneQuery
// {
//     glm::vec3 Size{};
//     glm::vec3 Position{};
//     glm::vec3 Rotation{};
//     glm::vec3 Color{};
//     std::shared_ptr<Texture2D> Texture{ nullptr };
// };

class Renderer2D
{
public: // experimental (do not deploy)
    static const std::shared_ptr<class Shader>& GetFlatShader() noexcept;

    static std::size_t GetTriangleCount() noexcept;

public:
    static bool Initialize();
    static void Shutdown();

    static void BeginScene(const Camera* camera);
    static void EndScene();

    static void DrawPlane(const glm::vec3& size, const glm::vec3& position, const glm::vec3& color);
    static void DrawPlane(const glm::vec3& size, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& color);
};

RENDERER_CODE_END
