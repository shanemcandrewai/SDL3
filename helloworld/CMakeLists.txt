#[[
cmake -B build -DCMAKE_CXX_COMPILER=clang++ -G "Ninja Multi-Config"

cmake --build build //lld-link: error: undefined symbol: WinMain
cmake --install build 

cmake --build build --config Release //lld-link: error: undefined symbol: WinMain
cmake --install build --config Release
]]

cmake_minimum_required(VERSION 3.23)

project(helloworld)

add_executable(${PROJECT_NAME} WIN32 helloworld.c)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set_property(CACHE CMAKE_INSTALL_PREFIX PROPERTY VALUE "${CMAKE_CURRENT_LIST_DIR}/install")
endif()

install(TARGETS ${PROJECT_NAME} DESTINATION .)
