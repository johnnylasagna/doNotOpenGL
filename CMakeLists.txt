cmake_minimum_required(VERSION 3.10)
project(DoNotOpenGL)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Define the global include directory for header files
include_directories(include)

# ---------------------------------------------------------
# 1. THE ENGINE CORE LIBRARY
# ---------------------------------------------------------
# Compile all core engine files into a static library (.lib or .a)
add_library(EngineCore STATIC
    src/core/application.cpp
    src/core/input.cpp
    src/core/errors.cpp
    src/core/window.cpp
    src/core/timer.cpp
    src/core/resourceManager.cpp
    src/graphics/camera.cpp
    src/graphics/mesh.cpp
    src/graphics/shader.cpp
    src/graphics/texture.cpp
    src/graphics/object.cpp
    
    # Include glad.c and stb_image.cpp if they are in your source tree
    src/glad/glad.c 
    src/graphics/stb.cpp
)

# Link standard system libraries to the engine
# Note: Adjust 'glfw3' or 'glfw' based on your specific OS/Package manager setup
find_package(OpenGL REQUIRED)
target_link_libraries(EngineCore PRIVATE OpenGL::GL glfw3) 

# ---------------------------------------------------------
# 2. THE GAME EXECUTABLE
# ---------------------------------------------------------
# Compile the specific game scenes and main entry point
add_executable(App
    src/main.cpp
    src/scenes/scene_2.cpp
)

# Link the pre-compiled EngineCore library to the executable
target_link_libraries(App PRIVATE EngineCore)

# Copy the assets directory to the build directory after compiling App
# Copy directories during the CMake configuration phase
add_custom_target(CopyAssets ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:App>/assets"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/shaders"
        "$<TARGET_FILE_DIR:App>/shaders"
)