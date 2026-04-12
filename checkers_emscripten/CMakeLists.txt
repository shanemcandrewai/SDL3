#[[

cmake -S ../../SDL -B build_libSDL3 -DSDL_TESTS=OFF -DSDL_TEST_LIBRARY=OFF -DCMAKE_INSTALL_PREFIX=install_libSDL3 -DCMAKE_TOOLCHAIN_FILE="../emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake" -G Ninja -DCMAKE_BUILD_TYPE=MinSizeRel

cmake --build build_libSDL3
cmake --install build_libSDL3

cmake -B build -DCMAKE_TOOLCHAIN_FILE="../../emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake" -G Ninja -DCMAKE_BUILD_TYPE=MinSizeRel
cmake --build build
cmake --install build

python -m http.server 8000
http://localhost:8000/

]]
cmake_minimum_required(VERSION 3.23)


project(checkers_emscripten)

# Set the name of the future application (in Windows this would be app.exe, while for web it will be app.js / app.wasm)
add_executable(${PROJECT_NAME} "${CMAKE_CURRENT_LIST_DIR}/../checkers.cpp")

# Tell CMake where to find the SDL3 library configuration files
set(SDL3_DIR "${CMAKE_CURRENT_LIST_DIR}/install_libSDL3/lib/cmake/SDL3")

# Load SDL3 package settings (compilation parameters and paths to headers)
find_package(SDL3 REQUIRED)

# Link SDL3 to our application (configures linking and include paths)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)

file(COPY ${CMAKE_CURRENT_LIST_DIR}/../blender/assets DESTINATION ${CMAKE_BINARY_DIR})
target_link_options(${PROJECT_NAME} PRIVATE "SHELL:--embed-file assets")

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set_property(CACHE CMAKE_INSTALL_PREFIX PROPERTY VALUE "${CMAKE_CURRENT_LIST_DIR}../../../shanemcandrewai.github.io")
endif()

install(TARGETS ${PROJECT_NAME} DESTINATION .)
install(FILES ${CMAKE_CURRENT_LIST_DIR}/index.html DESTINATION .)
install(FILES "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.wasm" DESTINATION .)
install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/../blender/assets DESTINATION .)
