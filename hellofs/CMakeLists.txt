#[[
cmake -B build -DCMAKE_CXX_COMPILER=clang++ -DSDL_STATIC=ON -DSDL_SHARED=OFF -G "Ninja Multi-Config"
cmake -B build -DCMAKE_CXX_COMPILER=clang++ -DSDL_STATIC=OFF -DSDL_SHARED=ON -G "Ninja Multi-Config"

cmake --build build //fatal error: 'emscripten.h' file not found
cmake --install build

cmake --build build --config Release //fatal error: 'emscripten.h' file not found
cmake --install build --config Release
]]

cmake_minimum_required(VERSION 3.23)

project(hellofs)

add_executable(${PROJECT_NAME} WIN32 hellofs.c)
add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL) #git clone https://github.com/libsdl-org/SDL
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set_property(CACHE CMAKE_INSTALL_PREFIX PROPERTY VALUE "${CMAKE_CURRENT_LIST_DIR}/install")
endif()

install(TARGETS ${PROJECT_NAME} DESTINATION .)
if(${SDL_SHARED})
  install(TARGETS SDL3-shared DESTINATION .)
endif()
