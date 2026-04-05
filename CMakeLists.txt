cmake_minimum_required(VERSION 3.21)
project(checkers)

# This assumes the SDL source is available in vendored/SDL
add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)

# Create your game executable target as usual
add_executable(${PROJECT_NAME} WIN32 checkers.cpp)

# Link to the actual SDL3 library.
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)

# Set the name of the future application (in Windows this would be app.exe, while for web it will be app.js / app.wasm)
add_executable(${PROJECT_NAME} checkers.cpp)

# emscripten

set(DCMAKE_TOOLCHAIN_FILE ../emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake)

# Tell CMake where to find the SDL3 library configuration files
set(SDL3_DIR install_libSDL3/lib/cmake/SDL3)

# Load SDL3 package settings (compilation parameters and paths to headers)
find_package(SDL3 REQUIRED)

# Link SDL3 to our application (configures linking and include paths)
target_link_libraries(checker_wasm PRIVATE SDL3::SDL3)

# Embed the image into the app.wasm file

target_link_options("checker_wasm" PRIVATE
"SHELL:--embed-file \"${CMAKE_CURRENT_SOURCE_DIR}/blender/blue.ortho.png@/blender/blue.ortho.png\""
"SHELL:--embed-file \"${CMAKE_CURRENT_SOURCE_DIR}/blender/cylinder.png@/blender/cylinder.png\""
)

set_property(TARGET "checker_wasm" APPEND PROPERTY LINK_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/blender/blue.ortho.png")
