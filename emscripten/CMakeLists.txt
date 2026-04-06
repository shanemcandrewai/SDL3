#[[
cmake -B build -G Ninja
cmake --build build

python -m http.server 8000
http://localhost:8000

]]
cmake_minimum_required(VERSION 3.21)

set(CMAKE_TOOLCHAIN_FILE "../../emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake")
project(hello_wasm)

set(CMAKE_BUILD_TYPE MinSizeRel)

# Set the name of the future application (in Windows this would be app.exe, while for web it will be app.js / app.wasm)
add_executable(${PROJECT_NAME} ../hello.cpp)

# Tell CMake where to find the SDL3 library configuration files
set(SDL3_DIR install_libSDL3/lib/cmake/SDL3)

# Load SDL3 package settings (compilation parameters and paths to headers)
find_package(SDL3 REQUIRED)

# Link SDL3 to our application (configures linking and include paths)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)

# Embed the image into the app.wasm file

target_link_options("hello_wasm" PRIVATE
"SHELL:--embed-file \"${CMAKE_CURRENT_SOURCE_DIR}/../blender/blue.ortho.png@/blender/blue.ortho.png\""
"SHELL:--embed-file \"${CMAKE_CURRENT_SOURCE_DIR}/../blender/cylinder.png@/blender/cylinder.png\""
)

set_property(TARGET "hello_wasm" APPEND PROPERTY LINK_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/../blender/blue.ortho.png")