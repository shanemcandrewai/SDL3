#[[

cmake -B build -DCMAKE_TOOLCHAIN_FILE="../../emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake" -G Ninja -DCMAKE_BUILD_TYPE=MinSizeRel
cmake --build build
cmake --install build

.\..\emsdk\emsdk activate latest
emcc -o ..\..\shanemcandrewai.github.io\test_html5_fullscreen.js .\test_html5_fullscreen.c
emcc -o ..\..\shanemcandrewai.github.io\test_html5_fullscreen.html .\test_html5_fullscreen.c

python -m http.server 8000
http://localhost:8000/

]]
cmake_minimum_required(VERSION 3.23)


project(test_html5_fullscreen)

add_executable(${PROJECT_NAME} "${CMAKE_CURRENT_LIST_DIR}/test_html5_fullscreen.c")

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set_property(CACHE CMAKE_INSTALL_PREFIX PROPERTY VALUE "${CMAKE_CURRENT_LIST_DIR}../../../shanemcandrewai.github.io")
endif()

install(TARGETS ${PROJECT_NAME} DESTINATION .)
install(FILES ${CMAKE_CURRENT_LIST_DIR}/test_html5_fullscreen.html DESTINATION .)
install(FILES "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.wasm" DESTINATION .)
