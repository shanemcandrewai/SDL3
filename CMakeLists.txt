cmake_minimum_required(VERSION 3.21)
project(checkers)

# cmake -B build -G "Ninja Multi-Config" -DCMAKE_CXX_COMPILER="D:\Program Files\LLVM\bin\clang++.exe" -DCMAKE_C_COMPILER="D:\Program Files\LLVM\bin\clang.exe"

# set(SDL_TESTS OFF)
# set(SDL_TEST_LIBRARY OFF)
# set(SDL_INSTALL_TESTS OFF)

# This assumes the SDL source is available in vendored/SDL
add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)

# Create your game executable target as usual
add_executable(${PROJECT_NAME} WIN32 checkers.cpp)

# Link to the actual SDL3 library.
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)

# emscripten

# Set the name of the future application (in Windows this would be app.exe, while for web it will be app.js / app.wasm)
add_executable(checkers_wasm checkers.cpp)

# Tell CMake where to find the SDL3 library configuration files
set(SDL3_DIR install_libSDL3/lib/cmake/SDL3)
set(CMAKE_BUILD_TYPE MinSizeRel)
set(DCMAKE_TOOLCHAIN_FILE "../emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake")

# Load SDL3 package settings (compilation parameters and paths to headers)
find_package(SDL3 REQUIRED)

# Embed the image into the app.wasm file
target_link_options("checkers" PRIVATE
"SHELL:--embed-file \"${CMAKE_CURRENT_SOURCE_DIR}/blender/blue.ortho.png@/blender/blue.ortho.png\""
"SHELL:--embed-file \"${CMAKE_CURRENT_SOURCE_DIR}/blender/cylinder.png@/blender/cylinder.png\""
)
set_property(TARGET "checkers" APPEND PROPERTY LINK_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/blender/blue.ortho.png")

