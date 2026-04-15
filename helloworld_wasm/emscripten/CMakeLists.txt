#[[
cmake -B build -DCMAKE_TOOLCHAIN_FILE="../../../emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake" -G Ninja -DCMAKE_BUILD_TYPE=MinSizeRel
cmake --build build && cmake --install build

rm build -r -force && cmake -B build -DCMAKE_TOOLCHAIN_FILE="../../../emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake" -G Ninja -DCMAKE_BUILD_TYPE=MinSizeRel && cmake --build build && cmake --install build

rm build -r -force && cmake -B build -DCMAKE_TOOLCHAIN_FILE="../../../emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake" -G Ninja && cmake --build build && cmake --install build

python -m http.server 8000
http://localhost:8000/

..\..\..\emsdk\emsdk activate latest
emcc -o hello.html ..\hello.cpp -sUSE_SDL=3
]]
cmake_minimum_required(VERSION 3.23)

project(hello_wasm)

add_executable(${PROJECT_NAME} "${CMAKE_CURRENT_LIST_DIR}/../${PROJECT_NAME}.cpp")
set(SDL3_DIR "${CMAKE_CURRENT_LIST_DIR}/../../SDL3_emsdk/install/lib/cmake/SDL3")
find_package(SDL3 REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set_property(CACHE CMAKE_INSTALL_PREFIX PROPERTY VALUE "${CMAKE_CURRENT_LIST_DIR}/../../../shanemcandrewai.github.io")
endif()

install(TARGETS ${PROJECT_NAME} DESTINATION . )
install(FILES ${CMAKE_CURRENT_LIST_DIR}/${PROJECT_NAME}.html DESTINATION .)
install(FILES ${CMAKE_CURRENT_LIST_DIR}/${PROJECT_NAME}_cust.js DESTINATION .)
#install(FILES ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.js RENAME ${PROJECT_NAME}_wasm.js DESTINATION .)
install(FILES ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.wasm DESTINATION .)
