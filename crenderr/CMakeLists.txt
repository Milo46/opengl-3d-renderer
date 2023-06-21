project(crenderr-lib)

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

add_library(${PROJECT_NAME} STATIC

    source/Crenderr/Utility/Checker.cpp
    source/Crenderr/Utility/FileManager.cpp

    source/Crenderr/Logger/Logger.cpp
    source/Crenderr/Filesystem/Filesystem.cpp

    source/Crenderr/Renderer/RendererCore.hpp
    source/Crenderr/Renderer/GraphicsContext.cpp
    source/Crenderr/Renderer/RenderCommand.cpp

    source/Crenderr/Renderer/Backend/Buffers.cpp
    source/Crenderr/Renderer/Backend/VertexArray.cpp
    source/Crenderr/Renderer/Backend/Texture2D.cpp
    source/Crenderr/Renderer/Backend/Framebuffer.cpp
    source/Crenderr/Renderer/Backend/Shader.cpp

    source/Crenderr/Renderer/Camera/Camera.cpp
    source/Crenderr/Renderer/Camera/Projections/Orthographic.cpp
    source/Crenderr/Renderer/Camera/Projections/Perspective.cpp
    source/Crenderr/Renderer/Camera/Controllers/CameraController.cpp

    source/Crenderr/Renderer/Loaders/OBJLoader.cpp

    source/Crenderr/Renderer/RendererElements.cpp
    source/Crenderr/Renderer/Renderer.cpp

    source/Crenderr/ImGui/ImGuiContext.cpp

    source/Crenderr/Window/Window.cpp
    source/Crenderr/Application/Scene.cpp
    source/Crenderr/Application/Application.cpp
    source/Crenderr/Core/EntryPoint.cpp

)

target_link_libraries(${PROJECT_NAME} PUBLIC
    glad
    glfw
    spdlog
    imgui
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${PROJECT_SOURCE_DIR}/vendor/entt/src
    ${PROJECT_SOURCE_DIR}/vendor/GLAD/include
    ${PROJECT_SOURCE_DIR}/vendor/glfw/include
    ${PROJECT_SOURCE_DIR}/vendor/glm
    ${PROJECT_SOURCE_DIR}/vendor/imgui
    ${PROJECT_SOURCE_DIR}/vendor/spdlog/include
    ${PROJECT_SOURCE_DIR}/vendor/stb

    ${PROJECT_SOURCE_DIR}/source/Crenderr
)
