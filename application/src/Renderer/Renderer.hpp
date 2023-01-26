#pragma once

#include "RendererCore.hpp"

#include "Renderer/RenderCommand.hpp"

#include "Renderer/Backend/Buffers.hpp"

#include "Renderer/Camera/Camera.hpp"
#include "Renderer/Camera/OrthographicCamera.hpp"
#include "Renderer/Camera/PerspectiveCamera.hpp"

NAMESPACE_BEGIN(Renderer)

struct RendererVertex
{
    glm::vec3 Position;
    glm::vec2 Texcoord;

    inline static const BufferLayout c_Layout{
        { LayoutDataType::Float3, "a_Position", },
        { LayoutDataType::Float2, "a_Texcoord", },
    };
};

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

    static void DrawPlane(const glm::vec3& size, const glm::vec3& position, const glm::vec3& color = glm::vec3(1.0f));
    static void DrawPlane(const glm::vec3& size, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& color);

    // Experimental
    static void DrawArrays(const std::shared_ptr<VertexArray>& vertexArray, std::size_t count);
};

NAMESPACE_END(Renderer)
