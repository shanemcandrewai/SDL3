# SDL3 development
## clang / VC++
### execute Visual Studio Installer
#### select desktop environment with C++
- MSVC Build Tools for x64/x86 (Latest)
- Windows 11 SDK
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
#### testing hello world
cls && clang -o hello.exe hello.c vendored\SDL\src\main\generic\SDL_sysmain_callbacks.c -I SDL3-3.4.2\include -I vendored\SDL\src -I vendored\SDL\include\build_config -Wl,/subsystem:console
#### lld-link: warning: found main and WinMain; defaulting to /subsystem:console
add linker option ```-Wl,/subsystem:console```
#### MSVC compiler
    cl hello.cpp SDL3.lib -I SDL3-3.4.2\include /link /LIBPATH:SDL3-3.4.2\lib\x64 /SUBSYSTEM:CONSOLE
## clang-tidy wip
    cls && clang-tidy hello.c -checks=* --header-filter=ddd -- -ISDL3-3.4.2\include
### using .clang-tidy
    cls && clang-tidy hello.cpp -- -ISDL3-3.4.2\include
    cls && clang-tidy hello.c -checks=-misc-unused-parameters,-cppcoreguidelines-avoid-non-const-global-variables -- -ISDL3-3.4.2\include
## clang-format
    clang-format -i hello.c
## Emscripten
https://github.com/libsdl-org/SDL/blob/main/docs/INTRO-emscripten.md
### link SDL directory to SDL3/vendored/SDL
The easiest way to use SDL is to include it as a subproject in your project.
#### execute cmd.exe as admin
    C:\Users\shane\dev\SDL3>mklink /d vendored\SDL ..\..\SDL
## msys2 gcc examples
    gcc -o build/hello.exe hello.c -ISDL3-3.4.2x86_64-w64-mingw32/include -LSDL3-3.4.2/x86_64-w64-mingw32/lib -lSDL3
    gcc -o build/woodeneye-008.exe -Wall -Wpedantic -Wextra woodeneye-008.c -ISDL3-3.4.2x86_64-w64-mingw32/include -lSDL3
### emcc
    emcc -o hello.exe hello.c vendored/SDL/src/*.c vendored/SDL/src/main/*.c vendored/SDL/src/video/*.c vendored/SDL/src/core/*.c vendored/SDL/src/render/*.c vendored/SDL/src/thread/*.c vendored/SDL/src/stdlib/*.c vendored/SDL/src/events/*.c vendored/SDL/src/filesystem/*.c  vendored/SDL/src/thread/windows/*.c -I vendored/SDL/include/ -I vendored/SDL/include/build_config/ -I vendored/SDL/src
### emcmake
	emcmake cmake -S vendored/SDL -B build -G "Unix Makefiles" -DSDL_TESTS:BOOL=OFF

