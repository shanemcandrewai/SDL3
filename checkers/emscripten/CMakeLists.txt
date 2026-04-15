#[[
cmake -B build -DCMAKE_TOOLCHAIN_FILE="../../../emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake" -G Ninja -DCMAKE_BUILD_TYPE=MinSizeRel
cmake --build build && cmake --install build

python -m http.server 8000
http://localhost:8000/

..\..\..\emsdk\emsdk activate latest
emcc -o checkers.js ..\checkers.cpp -sUSE_SDL=3
emcc -o checkers.html ..\checkers.cpp -sUSE_SDL=3
]]
cmake_minimum_required(VERSION 3.23)

project(checkers)

add_executable(${PROJECT_NAME} "${CMAKE_CURRENT_LIST_DIR}/../${PROJECT_NAME}.cpp")
set(SDL3_DIR "${CMAKE_CURRENT_LIST_DIR}/../../SDL3_emsdk/install/lib/cmake/SDL3")
find_package(SDL3 REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)

file(COPY ${CMAKE_CURRENT_LIST_DIR}/../../blender/assets DESTINATION ${CMAKE_BINARY_DIR})
target_link_options(${PROJECT_NAME} PRIVATE "SHELL:--embed-file assets")

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set_property(CACHE CMAKE_INSTALL_PREFIX PROPERTY VALUE "${CMAKE_CURRENT_LIST_DIR}/../../../shanemcandrewai.github.io")
endif()

install(TARGETS ${PROJECT_NAME} DESTINATION .)
install(FILES ${CMAKE_CURRENT_LIST_DIR}/index.html DESTINATION .)
install(FILES ${CMAKE_CURRENT_LIST_DIR}/index.html RENAME ${PROJECT_NAME}.html DESTINATION .)
install(FILES ${CMAKE_CURRENT_LIST_DIR}/${PROJECT_NAME}_cust.js DESTINATION .)
install(FILES "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.wasm" DESTINATION .)
