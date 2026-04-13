#[[

cmake -B build -DCMAKE_TOOLCHAIN_FILE="../../emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake" -G Ninja -DCMAKE_BUILD_TYPE=MinSizeRel
cmake --build build
cmake --install build

..\..\emsdk\emsdk activate latest
emcc -o ..\..\shanemcandrewai.github.io\hellofs.js .\hellofs.c -sUSE_SDL=3
emcc -o ..\..\shanemcandrewai.github.io\hellofs.html .\hellofs.c -sUSE_SDL=3

python -m http.server 8000
http://localhost:8000/

]]
cmake_minimum_required(VERSION 3.23)


project(hellofs)

add_executable(${PROJECT_NAME} hellofs.c)
set(SDL3_DIR "${CMAKE_CURRENT_LIST_DIR}/install_libSDL3/lib/cmake/SDL3")
find_package(SDL3 REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set_property(CACHE CMAKE_INSTALL_PREFIX PROPERTY VALUE "${CMAKE_CURRENT_LIST_DIR}../../../shanemcandrewai.github.io")
endif()

install(TARGETS ${PROJECT_NAME} DESTINATION .)
install(FILES ${CMAKE_CURRENT_LIST_DIR}/index.html DESTINATION .)
install(FILES "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.wasm" DESTINATION .)
