#[[
downloads -

https://github.com/emscripten-core/emsdk/releases/tag/5.0.4
https://github.com/libsdl-org/SDL/releases/tag/release-3.4.2
https://github.com/libsdl-org/SDL_image/releases/tag/release-3.4.0
https://github.com/ninja-build/ninja/releases/download/v1.13.2/ninja-win.zip

unzip to ninja\ninja.exe
add ninja\ to PATH

..\emsdk\emsdk.bat install latest
..\emsdk\emsdk.bat activate latest

emcmake cmake -S ../SDL -B build_libSDL3 -DSDL_TESTS:BOOL=OFF -DSDL_TEST_LIBRARY:BOOL=OFF -DCMAKE_BUILD_TYPE=MinSizeRel -DCMAKE_INSTALL_PREFIX=install_libSDL3

cmake --build build_libSDL3
cmake --install build_libSDL3

emcmake cmake -S ../SDL_image -B build_libSDL3_image -DCMAKE_BUILD_TYPE=MinSizeRel -DCMAKE_INSTALL_PREFIX=install_libSDL3_image

cmake --build build_libSDL3_image
cmake --install build_libSDL3_image

emcmake cmake -S . -B build
cmake --build build

python -m http.server 8000
http://localhost:8000/emscripten.html

]]
cmake_minimum_required(VERSION 3.21)

project(checkers)

# Set the C standard (must be set before add_executable)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Set the name of the future application (in Windows this would be app.exe, while for web it will be app.js / app.wasm)
add_executable(${PROJECT_NAME} checkers.cpp)

# Tell CMake where to find the SDL3 library configuration files
# set(SDL3_DIR SDL3-3.4.2/cmake)
set(SDL3_DIR install_libSDL3/lib/cmake/SDL3)
set(SDL3_image_DIR install_libSDL3_image/lib/cmake/SDL3_image)

# Load SDL3 package settings (compilation parameters and paths to headers)
find_package(SDL3 REQUIRED)
find_package(SDL3_image REQUIRED)

# Link SDL3 to our application (configures linking and include paths)
# target_link_libraries(app PRIVATE SDL3::SDL3)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3_image::SDL3_image SDL3::SDL3)

# Embed the image into the app.wasm file
if (EMSCRIPTEN)
    # Use the SHELL: prefix to pass the --embed-file option to the linker
    target_link_options("checkers" PRIVATE "SHELL:--embed-file \"${CMAKE_CURRENT_SOURCE_DIR}/build/blender/blue.ortho.png\"")
 
    # Ensure the target is relinked if the image file changes
    set_property(TARGET "checkers" APPEND PROPERTY LINK_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/build/blender/blue.ortho.png")
endif()
