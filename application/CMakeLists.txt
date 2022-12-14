project(OpenGL3DRenderer VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME}
    src/Utility/FileManager.hpp
    src/Utility/FileManager.cpp
    src/Utility/Checker.hpp
    src/Utility/Checker.cpp
    src/Utility/NonCopyable.hpp

    src/Logger/Logger.hpp
    src/Logger/Logger.cpp

    src/ImGui/ImGuiBuild.hpp
    src/ImGui/ImGuiContext.hpp
    src/ImGui/ImGuiContext.cpp

    src/Window/Window.hpp
    src/Window/Window.cpp

    src/Renderer/Camera/OrthographicCamera.cpp
    src/Renderer/Camera/PerspectiveCamera.cpp

    src/Renderer/GraphicsContext.hpp
    src/Renderer/GraphicsContext.cpp
    src/Renderer/RendererCore.hpp
    src/Renderer/RendererCore.cpp
    src/Renderer/Bindable.hpp
    src/Renderer/Shader.hpp
    src/Renderer/Shader.cpp
    src/Renderer/Texture2D.hpp
    src/Renderer/Texture2D.cpp
    src/Renderer/Buffers.hpp
    src/Renderer/Buffers.cpp
    src/Renderer/VertexArray.hpp
    src/Renderer/VertexArray.cpp
    src/Renderer/RenderCommand.hpp
    src/Renderer/RenderCommand.cpp
    src/Renderer/Renderer.cpp
    src/Renderer/Renderer.hpp

    src/Application/Application.hpp
    src/Application/Application.cpp

    src/Core/EntryPoint.hpp
    src/Core/EntryPoint.cpp
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    glad
    glfw
    spdlog
    imgui
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_SOURCE_DIR}/vendor/GLAD/include
    ${CMAKE_SOURCE_DIR}/vendor/GLFW/include
    ${CMAKE_SOURCE_DIR}/vendor/glm
    ${CMAKE_SOURCE_DIR}/vendor/imgui
    ${CMAKE_SOURCE_DIR}/vendor/spdlog/include
    ${CMAKE_SOURCE_DIR}/vendor/stb

    ${PROJECT_SOURCE_DIR}/src
)
