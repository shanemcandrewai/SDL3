#[[

cmake -B build -DCMAKE_CXX_COMPILER="C:/Program Files/LLVM/bin/clang++.exe" -DSDL_STATIC=ON -DSDL_SHARED=OFF -G "Ninja Multi-Config" 
cmake -B build -DCMAKE_CXX_COMPILER="C:/Program Files/LLVM/bin/clang++.exe" -DSDL_STATIC=OFF -DSDL_SHARED=ON -G "Ninja Multi-Config" 

cmake --build build
cmake --install build

cmake --build build --config Release
cmake --install build --config Release

.\install\checkers_clang.exe

]]

cmake_minimum_required(VERSION 3.23)

project(checkers_clang)

add_executable(${PROJECT_NAME} WIN32)

target_sources(${PROJECT_NAME} PRIVATE ../checkers.cpp)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_CURRENT_LIST_DIR}/install)
install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/../blender/assets DESTINATION ${CMAKE_CURRENT_LIST_DIR}/install)
