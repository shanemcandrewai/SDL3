# SDL3 development
## clang / VC++
### execute Visual Studio Installer
#### select desktop environment with C++
- Windows 11 SDK
- MSVC Build Tools for x64/x86 (Latest)
### install clang / LLVM
https://github.com/llvm/llvm-project/releases/download/llvmorg-22.1.1/LLVM-22.1.1-win64.exe
### add clang tools to path
    C:\Program Files\LLVM\bin
## download SDL3 libraries and headers, unzip in executable's directory
https://github.com/libsdl-org/SDL/releases/download/release-3.4.2/SDL3-devel-3.4.2-VC.zip
## copy to SDL3.dll to executable's directory
    copy SDL3-3.4.2\lib\x64\SDL3.dll .
## Execute x64 Native Tools Command Prompt for VS
    %comspec% /k "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvars64.bat"
### compile
    clang -o hello.exe hello.c -I SDL3-3.4.2\include -l SDL3-3.4.2\lib\x64\SDL3 -Wl,/subsystem:console
#### lld-link: warning: found main and WinMain; defaulting to /subsystem:console
add linker option ```-Wl,/subsystem:console```
#### MSVC compiler
    cl hello.cpp SDL3.lib -I SDL3-3.4.2\include /link /LIBPATH:SDL3-3.4.2\lib\x64 /SUBSYSTEM:CONSOLE
## ninja
https://github.com/ninja-build/ninja/releases/download/v1.13.2/ninja-win.zip
### installation
- unzip to ninja-win\ninja.exe
- add ninja-win to PATH
## SDL3
https://github.com/libsdl-org/SDL
### checkout latest releases
    git checkout release-3.4.4
### link SDL directory to SDL3/vendored/SDL
[The easiest way to use SDL is to include it as a subproject in your project](https://github.com/libsdl-org/SDL/blob/main/docs/INTRO-emscripten.md)
#### execute cmd.exe as admin
    C:\Users\shane\dev\SDL3>mklink /d vendored\SDL ..\..\SDL
## cmake
### cmake-gui
#### Where is the source code
    C:/Users/shane/dev/SDL3
#### Where to build the binaries
    C:/Users/shane/dev/SDL3/build
### configure
#### Specify the generator for this project
    Ninja Multi-config
#### Specify native compilers
##### C
    D:/Program Files/LLVM/bin/clang.exe
##### C++
    D:/Program Files/LLVM/bin/clang++.exe
### CMAKE_INSTALL_PREFIX
    C:/Users/shane/dev/SDL3/install
### configure (again)
All red entries should be gone
### generate
#### alternatively, from command line
##### after cmake-gui config
    cmake -B build
### cmake command
#### generate project files
    cmake -B build -G "Ninja Multi-Config" -DCMAKE_CXX_COMPILER="D:\Program Files\LLVM\bin\clang++.exe" -DCMAKE_C_COMPILER="D:\Program Files\LLVM\bin\clang.exe"
#### build
    cmake --build build [--config Release]
#### copy SDL.dll
    copy build\vendored\SDL\Release\SDL3.dll .
## emscripten
https://discourse.libsdl.org/t/how-to-compile-sdl3-hello-world-to-webassembly-with-emscripten/66858/6
### downloads
#### emsdk
https://github.com/emscripten-core/emsdk
##### checkout latest release
    git pull
    git checkout 5.05
##### install latest emscripten build
    ./emsdk install latest
### build SDL3 libary
#### generate project buildsystem
    cmake -S ../SDL -B build_libSDL3 -DSDL_TESTS:BOOL=OFF -DSDL_TEST_LIBRARY:BOOL=OFF -DCMAKE_BUILD_TYPE=MinSizeRel -DCMAKE_INSTALL_PREFIX=install_libSDL3 -DCMAKE_TOOLCHAIN_FILE=../emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake -G Ninja
#### build project
    cmake --build build_libSDL3
#### install project (need cmake files in install_libSDL3\lib\cmake\SDL3 for WASM generation)
    cmake --install build_libSDL3
### generate WASM and js files from cpp
#### sample emscripten project CMakeLists.txt
	cmake_minimum_required(VERSION 3.21)
	project(checkers)
	add_executable(${PROJECT_NAME} checkers.cpp)
	set(SDL3_DIR install_libSDL3/lib/cmake/SDL3)
	find_package(SDL3 REQUIRED)
	target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)
	target_link_options("checkers" PRIVATE
	"SHELL:--embed-file \"${CMAKE_CURRENT_SOURCE_DIR}/blender/blue.ortho.png@/blender/blue.ortho.png\""
	"SHELL:--embed-file \"${CMAKE_CURRENT_SOURCE_DIR}/blender/cylinder.png@/blender/cylinder.png\""
	)
    set_property(TARGET "checkers" APPEND PROPERTY LINK_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/blender/blue.ortho.png")
#### generate project buildsystem
    cmake -B build -DCMAKE_BUILD_TYPE=MinSizeRel -DCMAKE_TOOLCHAIN_FILE=../emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake -G Ninja
#### build project
    cmake --build build
#### alternative without building SDL3
- https://emscripten.org/docs/tools_reference/settings_reference.html#use-sdl
- https://stackoverflow.com/questions/79914903/how-to-compile-sdl3-to-webassembly-with-emscripten
##### emcc ports
    emcc --show-ports
    -sUSE_SDL=3
#### create index.html
	<!DOCTYPE html>
	<html lang="en">
	   <head>
		  <meta charset="UTF-8">
		  <meta name="viewport" content="width=device-width,initial-scale=1">
		  <title>SDL3 emscripten</title>
		  <script async type="text/javascript" src="build/checkers.js"></script>
	   </head>
	   <body>
		  <div class="emscripten_border">
			 <canvas class="emscripten" id="canvas"
				oncontextmenu="event.preventDefault()"
				tabindex=-1>
			 </canvas>
		  </div>
		  <textarea id="output" rows="8"></textarea>
	   </body>
	</html>
#### test
    python -m http.server 8000
##### open browser URL
    http://localhost:8000
## clang-repl
### windows set up for precompiled SDL3 libaries
    set CPLUS_INCLUDE_PATH=SDL3-3.4.2/include
	clang-repl
	%lib SDL3-3.4.2/lib/x64/SDL3.dll
### environmental library search path -L  passed on to the linker
    LIBRARY_PATH
### environmental include search path -I
    C_INCLUDE_PATH
    CPLUS_INCLUDE_PATH
#### link to DLL
    %lib SDL3-3.4.2/lib/x64/SDL3.dll
#### disply environment variable in c
    const char *p = getenv("PATH");
	p [Enter]
### windows
#### display ENVIRONMENT_VARIABLE
    echo %ENVIRONMENT_VARIABLE%
#### set ENVIRONMENT_VARIABLE to "value"
    set ENVIRONMENT_VARIABLE=value
## clang-tidy wip
    cls && clang-tidy hello.c -checks=* --header-filter=ddd -- -ISDL3-3.4.2\include
### using .clang-tidy
    cls && clang-tidy hello.cpp -- -ISDL3-3.4.2\include
    cls && clang-tidy hello.c -checks=-misc-unused-parameters,-cppcoreguidelines-avoid-non-const-global-variables -- -ISDL3-3.4.2\include
## clang-format
    clang-format -i hello.c
## lldb (clang debugger)
### error: unable to find 'python311.dll'.
#### solution
##### install python 3.11
    py install 3.11
##### create virtual enviroment
    C:\Users\shane\AppData\Local\Python\bin\python3.11.exe -m venv ..\venv\3.11
    ..\venv\3.11\Scripts\activate
##### add dll to path
    set PATH=%PATH%;%LocalAppData%\Python\pythoncore-3.11-64
## msys2 gcc examples
    gcc -o build/hello.exe hello.c -ISDL3-3.4.2x86_64-w64-mingw32/include -LSDL3-3.4.2/x86_64-w64-mingw32/lib -lSDL3
    gcc -o build/woodeneye-008.exe -Wall -Wpedantic -Wextra woodeneye-008.c -ISDL3-3.4.2x86_64-w64-mingw32/include -lSDL3
### emcc
    emcc -o hello.exe hello.c vendored/SDL/src/*.c vendored/SDL/src/main/*.c vendored/SDL/src/video/*.c vendored/SDL/src/core/*.c vendored/SDL/src/render/*.c vendored/SDL/src/thread/*.c vendored/SDL/src/stdlib/*.c vendored/SDL/src/events/*.c vendored/SDL/src/filesystem/*.c  vendored/SDL/src/thread/windows/*.c -I vendored/SDL/include/ -I vendored/SDL/include/build_config/ -I vendored/SDL/src
### emcmake
	emcmake cmake -S vendored/SDL -B build -G "Unix Makefiles" -DSDL_TESTS:BOOL=OFF
### woodeneye-008.touch.c
    emcc woodeneye-008.touch.c -sUSE_SDL=3 && copy /y a.* ..\shanemcandrewai.github.io && cd ..\shanemcandrewai.github.io && git commit -am 'next' && git push && cd ..\SDL3
### cmake
#### create link vendored\SDL to ..\SDL repo
##### cmd admin
    mklink /D vendored\SDL ..\..\SDL

