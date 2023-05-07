    #pragma once

#include "RendererCore.hpp"

#include "Renderer/RenderCommand.hpp"
#include "Renderer/RendererElements.hpp"

#include "Renderer/Backend/Buffers.hpp"
#include "Renderer/Backend/Shader.hpp"

#include "Renderer/Camera/Camera.hpp"

NAMESPACE_BEGIN(Renderer)

class RendererInstance
{
public:
    virtual bool OnInitialization() noexcept = 0;
    virtual void OnShutdown() noexcept = 0;

    virtual void BeginScene(Camera* camera) noexcept = 0;
    virtual void EndScene() noexcept = 0;
};

struct Renderer3DStorage;

class Renderer3DInstance : public RendererInstance
{
public: // experimental
    const std::shared_ptr<Shader>& GetFlatShader() const noexcept;
    
    std::size_t GetPrimitivesRendered() const noexcept;

public:
    virtual bool OnInitialization() noexcept override;
    virtual void OnShutdown() noexcept override;

public:
    virtual void BeginScene(Camera* camera) noexcept override;
    virtual void EndScene() noexcept override;

public:
    void DrawPlane(const Translation& translation);
    void DrawCube(const Translation& translation, const glm::vec3& color = glm::vec3(1.0f));

    void SetPointLight(const glm::vec3& position, const glm::vec3& color);

    void DrawArrays(
        const std::shared_ptr<VertexArray>& vertexArray,
        const std::shared_ptr<Texture2D>& diffuse,
        const std::shared_ptr<Texture2D>& specular,
        const std::shared_ptr<Texture2D>& emission,
        bool wireframe = false);

public:
    // std::unique_ptr<Renderer3DStorage> m_Storage;
    Renderer3DStorage* m_Storage{};
};

template<typename _Ty>
using ResourceHandle = std::shared_ptr<_Ty>;

NAMESPACE_END(Renderer)
