cmake_minimum_required(VERSION 3.31)
project(3DRenderer)

set(CMAKE_CXX_STANDARD 20)


# Enable FetchContent
include(FetchContent)

# Fetch GLFW
FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG latest
)
FetchContent_MakeAvailable(glfw)

add_library(glad STATIC external/glad/src/glad.c)
target_include_directories(glad PUBLIC external/glad/include)

# Fetch GLM
FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG 0.9.9.8
)
FetchContent_MakeAvailable(glm)

# Fetch stb (header only, just needed as source dir)
FetchContent_Declare(
        stb
        GIT_REPOSITORY https://github.com/nothings/stb.git
)
FetchContent_MakeAvailable(stb)

add_executable(3DRenderer
        src/main.cpp
        src/Window.cpp
)

# Include directories
target_include_directories(3DRenderer PRIVATE
        ${glfw_SOURCE_DIR}/include
        ${glm_SOURCE_DIR}
        ${stb_SOURCE_DIR}
)

# Link libraries
target_link_libraries(3DRenderer
        glfw
        glad
        glm
        ${OPENGL_gl_LIBRARY}
)

# On some systems, you may need to link OpenGL manually
find_package(OpenGL REQUIRED)
target_link_libraries(3DRenderer ${OPENGL_gl_LIBRARY})