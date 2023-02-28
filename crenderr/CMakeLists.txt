project(crenderr)

# GLFW options
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# 3rd party libraries
add_subdirectory(vendor/entt)
add_subdirectory(vendor/GLAD)
add_subdirectory(vendor/glfw)
add_subdirectory(vendor/glm)
add_subdirectory(vendor/imgui)
add_subdirectory(vendor/spdlog)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(${PROJECT_NAME}-lib STATIC

    source/Utility/Checker.cpp
    source/Utility/FileManager.cpp

    source/Logger/Logger.cpp
    source/Filesystem/Filesystem.cpp

    source/Renderer/RendererCore.hpp
    source/Renderer/GraphicsContext.cpp
    source/Renderer/RenderCommand.cpp

    source/Renderer/Backend/Buffers.cpp
    source/Renderer/Backend/VertexArray.cpp
    source/Renderer/Backend/Texture2D.cpp
    source/Renderer/Backend/Framebuffer.cpp
    source/Renderer/Backend/Shader.cpp

    source/Renderer/Camera/OrthographicCamera.cpp
    source/Renderer/Camera/PerspectiveCamera.cpp

    source/Renderer/Loaders/OBJLoader.cpp

    source/Renderer/Renderer.cpp

    source/ImGui/ImGuiContext.cpp

    source/Window/Window.cpp
    source/Application/Scene.cpp
    source/Application/Application.cpp
    source/Core/EntryPoint.cpp

)

target_link_libraries(${PROJECT_NAME}-lib PUBLIC
    glad
    glfw
    spdlog
    imgui
)

target_include_directories(${PROJECT_NAME}-lib PUBLIC
    ${PROJECT_SOURCE_DIR}/vendor/entt/src
    ${PROJECT_SOURCE_DIR}/vendor/GLAD/include
    ${PROJECT_SOURCE_DIR}/vendor/glfw/include
    ${PROJECT_SOURCE_DIR}/vendor/glm
    ${PROJECT_SOURCE_DIR}/vendor/imgui
    ${PROJECT_SOURCE_DIR}/vendor/spdlog/include
    ${PROJECT_SOURCE_DIR}/vendor/stb

    ${PROJECT_SOURCE_DIR}/source
)
