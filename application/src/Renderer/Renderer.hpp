#pragma once

#include "RendererCore.hpp"

#include "Renderer/RenderCommand.hpp"

#include "Renderer/Backend/Buffers.hpp"
#include "Renderer/Backend/Shader.hpp"

#include "Renderer/Camera/Camera.hpp"
#include "Renderer/Camera/OrthographicCamera.hpp"
#include "Renderer/Camera/PerspectiveCamera.hpp"

NAMESPACE_BEGIN(Renderer)

struct Renderer
{
    virtual bool OnInitialization() noexcept = 0;
    virtual void OnShutdown() noexcept = 0;

    virtual void BeginScene(Camera* camera) noexcept = 0;
    virtual void EndScene() noexcept = 0;
};

struct Translation
{
    glm::vec3 Scale    = { glm::vec3(1.0f) };
    glm::vec3 Position = { glm::vec3(0.0f) };
    glm::vec3 Rotation = { glm::vec3(0.0f) }; // 0.0f - 360.0f (deg.)

    glm::mat4 ComposeModelMatrix() const noexcept;
};

struct Renderer3DStorage;

class Renderer3D : public Renderer
{
public: // experimental
    const std::shared_ptr<Shader>& GetFlatShader() const noexcept;
    
    std::size_t GetPrimitivesRendered() const noexcept;

public:
    struct Vertex
    {
        glm::vec3 Position{};
        glm::vec3 Normal{};
        glm::vec2 Texcoord{};

        inline static const BufferLayout c_Layout{
            { LayoutDataType::Float3, "a_Position", },
            { LayoutDataType::Float3, "a_Normal",   },
            { LayoutDataType::Float2, "a_Texcoord", },
        };
    };

public:
    virtual bool OnInitialization() noexcept override;
    virtual void OnShutdown() noexcept override;

public:
    virtual void BeginScene(Camera* camera) noexcept override;
    virtual void EndScene() noexcept override;

public:
    void DrawPlane(const Translation& translation);
    void DrawCube(const Translation& translation, const glm::vec3& color = glm::vec3(1.0f));

    void SetLightPosition(const glm::vec3& position);

    void DrawArrays(
        const std::shared_ptr<VertexArray>& vertexArray,
        const std::shared_ptr<Texture2D>& diffuse,
        const std::shared_ptr<Texture2D>& specular,
        const std::shared_ptr<Texture2D>& emission,
        bool wireframe = false);

public:
    std::unique_ptr<Renderer3DStorage> m_Storage{};
};

template<typename _Ty>
using ResourceHandle = std::shared_ptr<_Ty>;

struct Renderer3DStorage
{
    ResourceHandle<VertexArray> PlaneVArray{};
    ResourceHandle<Shader> FlatShader{};

    ResourceHandle<Texture2D> FlatTexture{};
    ResourceHandle<Texture2D> CubeTexture{};

    std::size_t PrimitivesCount{ 0u };
    std::size_t PrimitivesCountTemp{ 0u };
};

NAMESPACE_END(Renderer)
