project(OpenGL3DRenderer VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME}
    src/ImGui/ImGuiBuild.hpp

    src/main.cpp
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
)
