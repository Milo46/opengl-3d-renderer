project(crenderr-application)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME}
    source/UserApplication.hpp
    source/UserApplication.cpp
    source/UserScene.hpp
    source/UserScene.cpp
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    crenderr-lib
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_SOURCE_DIR}/crenderr/source

    ${CMAKE_SOURCE_DIR}/crenderr/vendor/entt/src
    ${CMAKE_SOURCE_DIR}/crenderr/vendor/GLAD/include
    ${CMAKE_SOURCE_DIR}/crenderr/vendor/glfw/include
    ${CMAKE_SOURCE_DIR}/crenderr/vendor/glm
    ${CMAKE_SOURCE_DIR}/crenderr/vendor/imgui
    ${CMAKE_SOURCE_DIR}/crenderr/vendor/spdlog/include
    ${CMAKE_SOURCE_DIR}/crenderr/vendor/stb
)
