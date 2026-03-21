cmake_minimum_required(VERSION 3.20)
#https://github.com/libsdl-org/SDL/blob/main/docs/INTRO-emscripten.md
# emsdk>emsdk.bat install latest
# SDL3>..\emsdk\emsdk.bat activate latest
# SDL3>emcmake cmake -S . -B build -G "Visual Studio 18 2026"
# SDL3>emcmake cmake --build build
project(hello)
# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

# This assumes the SDL source is available in vendored/SDL
add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)

# on Web targets, we need CMake to generate a HTML webpage.
if(EMSCRIPTEN)
  set(CMAKE_EXECUTABLE_SUFFIX ".html" CACHE INTERNAL "")
endif()

# Create your game executable target as usual
add_executable(hello WIN32 hello.c)

# Link to the actual SDL3 library.
target_link_libraries(hello PRIVATE SDL3::SDL3)