Hello,
I have two working CMakeList.txt files -

```<project root>CMakeList.txt```

```
#[[

# build with shared prebuilt SDL3 library (DLL)

cmake -B build -G "Ninja Multi-Config" 

cmake --build build
build\Debug\hello.exe

cmake --build build --config Release
build\Release\hello.exe

]]

cmake_minimum_required(VERSION 3.20)

project(hello)

set(CMAKE_C_COMPILER "C:/Program Files/LLVM/bin/clang.exe")
set(CMAKE_CXX_COMPILER "C:/Program Files/LLVM/bin/clang++.exe")


add_library(SDL3 SHARED IMPORTED)
set_target_properties(SDL3 PROPERTIES
INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/SDL3-3.4.2/include"
IMPORTED_IMPLIB "${CMAKE_SOURCE_DIR}/SDL3-3.4.2/lib/x64/SDL3.lib" # SHARED
)

add_executable(${PROJECT_NAME} hello.cpp)
target_link_libraries(${PROJECT_NAME} SDL3)

#set(ENV{CMAKE_GENERATOR} Ninja)
#add_subdirectory(emscripten)
```

which builds a SDL3 Hello, World! C++ program using generator (-G) Ninja Multi-Config and


```<project root>/emscripten/CMakeList.txt```

```
#[[
cmake -B build -G Ninja
cmake --build build

python -m http.server 8000
http://localhost:8000

]]
cmake_minimum_required(VERSION 3.21)

set(CMAKE_TOOLCHAIN_FILE "../../emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake")
project(hello_wasm)

set(CMAKE_BUILD_TYPE MinSizeRel)

# Set the name of the future application (in Windows this would be app.exe, while for web it will be app.js / app.wasm)
add_executable(${PROJECT_NAME} ../hello.cpp)

# Tell CMake where to find the SDL3 library configuration files
set(SDL3_DIR install_libSDL3/lib/cmake/SDL3)

# Load SDL3 package settings (compilation parameters and paths to headers)
find_package(SDL3 REQUIRED)

# Link SDL3 to our application (configures linking and include paths)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)

# Embed the image into the app.wasm file

target_link_options("hello_wasm" PRIVATE
"SHELL:--embed-file \"${CMAKE_CURRENT_SOURCE_DIR}/../blender/blue.ortho.png@/blender/blue.ortho.png\""
"SHELL:--embed-file \"${CMAKE_CURRENT_SOURCE_DIR}/../blender/cylinder.png@/blender/cylinder.png\""
)

set_property(TARGET "hello_wasm" APPEND PROPERTY LINK_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/../blender/blue.ortho.png")
```

builds the same  C++ SDL3 Hello, World! as emscripten (javascript and WASM files)
using generator (-G) Ninja 

I want to control the entire build from a single CMakeList.txt, so I thought to trigger emscripten build from the end of the C++ build by adding the following to the end of ```<project root>CMakeList.txt```
```
set(ENV{CMAKE_GENERATOR} Ninja)
add_subdirectory(emscripten)
```
However it fails with errors -
```
CMake Error at emscripten/CMakeLists.txt:23 (find_package):
  Could not find a configuration file for package "SDL3" that is compatible
  with requested version "".

  The following configuration files were considered but not accepted:

    <project root>emscripten/install_libSDL3/lib/cmake/SDL3/SDL3Config.cmake, version: 3.4.2 (32bit)
      The version found is not compatible with the version requested.

```
Is there a way I can control the entire build from a main ```CMakeList.txt``?