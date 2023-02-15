cmake_minimum_required(VERSION 3.16)

# GLFW options
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# 3rd party libraries
add_subdirectory(vendor/entt)
add_subdirectory(vendor/GLAD)
add_subdirectory(vendor/GLFW)
add_subdirectory(vendor/glm)
add_subdirectory(vendor/imgui)
add_subdirectory(vendor/spdlog)

# Application directory
add_subdirectory(application)
