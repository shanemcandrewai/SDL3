cmake_minimum_required(VERSION 3.21)
project(hello-wasm-sdl3-c)

# Set the name of the future application (in Windows this would be app.exe, while for web it will be app.js / app.wasm)
add_executable(app)

# Set the C standard
set(CMAKE_C_STANDARD 11)

# Tell CMake where to find the SDL3 library configuration files
set(SDL3_DIR build/lib/cmake/SDL3)

# Load SDL3 package settings (compilation parameters and paths to headers)
find_package(SDL3 REQUIRED)

# Link SDL3 to our application (configures linking and include paths)
target_link_libraries(app PRIVATE SDL3::SDL3)

# Add source code to the project
target_sources(app
    PRIVATE
    hello.cpp)