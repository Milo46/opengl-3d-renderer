project(glad)

add_library(glad STATIC
    src/glad.c
)

target_include_directories(glad PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)
