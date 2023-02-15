// // ApplicationPrototype.cpp

// class Application;

// struct Timestamp
// {
//     float DeltaTime;
//     float TotalTime;
//     float PreviousFrame;
// };

// class State
// {
//     friend Application;

// protected:
//     explicit State(const std::string_view name);

//     virtual void OnInitialization() = 0;
//     virtual void OnDestruction() = 0;

//     virtual void OnUpdate(const Timestamp& timestamp) = 0;
// };

// class ApplicationViewportPanel : public State
// {
// protected:
//     explicit ApplicationViewportPanel(const std::string_view name)
//         : State{ name } {}

//     virtual void OnInitialization() override
//     {
//         // #1. Setup renderer if not already set.
//         // #2. Prepare assets: meshes, materials, cameras...
//     }

//     virtual void OnDestruction() override
//     {
//         // #1. Release all resources and than systems.
//     }

//     virtual void OnUpdate(const Timestamp& timestamp) override
//     {
        
//     }

// private:
//     Renderer::OrthographicCamera m_Camera{};

//     std::size_t TriangleCount{ 0u };
//     bool m_WireframeMode{ false };
// };

// class Application
// {
// public:
//     explicit Application(const ApplicationProps& props) noexcept;
//     ~Application();

//     bool Initialize() noexcept;
//     void Run() noexcept;
//     void Shutdown() noexcept;

// private:
//     void OnUpdate(const Timestamp& timestamp) noexcept;
//     void OnRenderViewport() noexcept;
//     void OnRenderImGui(ImGuiIO& io, const Timestamp& timestamp) noexcept;

// private:
//     void PanelViewport(ImGuiIO& io, const Timestamp& timestamp) noexcept;
//     void PanelShader(ImGuiIO& io, const Timestamp& timestamp) noexcept;

// private:
//     std::unique_ptr<Window> m_Window;
//     std::unique_ptr<ImGuiBuildContext> m_ImGuiContext;

//     Renderer::OrthographicCamera m_Camera;

//     std::size_t m_TriangleCount{ 0u };
//     bool m_WireframeMode{ false };

//     // ImGui viewport assets
//     Renderer::RendererID m_Framebuffer{ 0u };
//     Renderer::RendererID m_Renderbuffer{ 0u };
//     std::shared_ptr<Renderer::Texture2D> m_TextureColorbuffer{};

//     // Custom va and shader assets
//     std::shared_ptr<Renderer::VertexArray> m_RectangleVA{};
//     std::shared_ptr<Renderer::Shader> m_CustomShader{};
//     Renderer::ShaderDataExtractor m_CustomShaderData{};
// };
