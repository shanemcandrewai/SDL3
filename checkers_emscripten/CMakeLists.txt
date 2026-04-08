#[[
cmake -B build -DCMAKE_TOOLCHAIN_FILE="../../emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake" -G Ninja
cmake --build build

python -m http.server 8000
http://localhost:8000/

]]
cmake_minimum_required(VERSION 3.25)


project(checkers_emscripten)

file(COPY ${CMAKE_CURRENT_LIST_DIR}/../blender/assets DESTINATION ${CMAKE_CURRENT_LIST_DIR})
file(GLOB_RECURSE data assets/*)

# Set the name of the future application (in Windows this would be app.exe, while for web it will be app.js / app.wasm)
add_executable(${PROJECT_NAME} "${CMAKE_CURRENT_LIST_DIR}/../checkers.cpp")

# Tell CMake where to find the SDL3 library configuration files
set(SDL3_DIR "${CMAKE_CURRENT_LIST_DIR}/install_libSDL3/lib/cmake/SDL3")
#set(SDL3_DIR "C:/Users/shane/dev/SDL3/emscripten/install_libSDL3/lib/cmake/SDL3")

# Load SDL3 package settings (compilation parameters and paths to headers)
find_package(SDL3 REQUIRED)

# Link SDL3 to our application (configures linking and include paths)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)


target_link_options("checkers_emscripten" PRIVATE 
"SHELL:--embed-file \"${CMAKE_CURRENT_LIST_DIR}/assets/blue.ortho.png@/assets/blue.ortho.png\""
"SHELL:--embed-file \"${CMAKE_CURRENT_LIST_DIR}/assets/cylinder.png@/assets/cylinder.png\""
)
 
set_property(TARGET "checkers_emscripten" APPEND PROPERTY LINK_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/assets/blue.ortho.png")
