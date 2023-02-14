project(OpenGL3DRenderer VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME}

    src/Utility/FileManager.cpp
    src/Utility/Checker.cpp

    src/Logger/Logger.cpp
    src/Filesystem/Filesystem.cpp
    src/ImGui/ImGuiContext.cpp

    src/Window/Window.cpp

    src/Renderer/Backend/Shader.cpp
    src/Renderer/Backend/Buffers.cpp
    src/Renderer/Backend/VertexArray.cpp
    src/Renderer/Backend/Texture2D.cpp
    src/Renderer/Backend/Framebuffer.cpp
    src/Renderer/Camera/OrthographicCamera.cpp
    src/Renderer/Camera/PerspectiveCamera.cpp
    src/Renderer/GraphicsContext.cpp
    src/Renderer/RenderCommand.cpp
    src/Renderer/Renderer.cpp

    src/Application/Application.cpp
    src/Application/OBJLoader.cpp
    src/Application/Scene.cpp

    src/Core/EntryPoint.cpp
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    glad
    glfw
    spdlog
    imgui
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_SOURCE_DIR}/vendor/entt/src
    ${CMAKE_SOURCE_DIR}/vendor/GLAD/include
    ${CMAKE_SOURCE_DIR}/vendor/GLFW/include
    ${CMAKE_SOURCE_DIR}/vendor/glm
    ${CMAKE_SOURCE_DIR}/vendor/imgui
    ${CMAKE_SOURCE_DIR}/vendor/spdlog/include
    ${CMAKE_SOURCE_DIR}/vendor/stb

    ${PROJECT_SOURCE_DIR}/src
)
