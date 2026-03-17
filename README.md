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
## download SDL3 libraries and headers
https://github.com/libsdl-org/SDL/releases/download/release-3.4.2/SDL3-devel-3.4.2-VC.zip
## Execute x64 Native Tools Command Prompt for VS
    %comspec% /k "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvars64.bat"
## copy to SDL3.dll to executable's directory
    copy SDL3-3.4.2\lib\x64\SDL3.dll .
### compile
    clang -o hello.exe hello.c -ISDL3-3.4.2\include -lSDL3-3.4.2\lib\x64\SDL3
## clang-tidy wip
    clang-tidy hello.c -checks=* --header-filter=ddd -- -ISDL3-devel-3.4.2-VC\SDL3-3.4.2\include
## msys2 gcc examples
    gcc -o build/hello.exe hello.c -ISDL3-3.4.2x86_64-w64-mingw32/include -LSDL3-3.4.2/x86_64-w64-mingw32/lib -lSDL3
    gcc -o build/woodeneye-008.exe -Wall -Wpedantic -Wextra woodeneye-008.c -ISDL3-3.4.2x86_64-w64-mingw32/include -lSDL3
