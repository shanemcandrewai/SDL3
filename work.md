# SDL3 project command lines
## clang-tidy
7808 warnings generated Suppressed 7814 warnings (7796 in non-user code, 18 NOLINT).

lldb
set PATH=%PATH%;%LocalAppData%\Python\pythoncore-3.11-64

  // if (!SDL_SetRenderLogicalPresentation(
  //         state->renderer,
  //         static_cast<int>(round(WIDTH * SPRITE_SCALE)),  // NOLINT
  //         static_cast<int>(round(HEIGHT * SPRITE_SCALE)), // NOLINT
  //         SDL_LOGICAL_PRESENTATION_STRETCH)) {
  //   SDL_Log("SDL_SetRenderLogicalPresentation failed: %s", // NOLINT
  //           SDL_GetError());
  // }

   <!-- <style>
      #canvas {
         transform: scale(1.5);
      }
   </style> -->

## lldb
```
lldb checkers.exe
b checkers.cpp:40
br l
br wr -f bp.txt
br re -f bp.txt
```
## screen resolution

pixel 8a 1080 x 2400 pixels, 20:9 ratio (~430 ppi density)
Lenovo 1920 x 1200 pixels 16:10 WUXGA

// const int static WIDTH_WUXGA = 1920;
// const int static HEIGHT_WUXGA = 1200;

Full HD 1920 × 1080p	16:9

const int static WIDTH = 1920;
const int static HEIGHT = 1080 ;
### SDL3>hello
```
cmake --build build --config Release && cmake --install build --config Release && cd install && .\hello.exe && cd ..
clang++  -Wall -Wextra -o hello.exe hello.cpp hello_internal.cpp -I"..\SDL3-3.4.4\include" -I. -l"..\SDL3-3.4.4\lib\x64\SDL3" -"Wl,/subsystem:console"
```
### emscripten>
```
emcc -Wall -Wextra -o checkers.html checkers.cpp -I"checkers_clang\SDL3-3.4.2\include" -sUSE_SDL=3
```
### SDL3>checkers>emscripten
```
cmake --build build && cmake --install build
```
### SDL3>
```
clang-format -i checkers\src\*.cpp
cls && clang-tidy checkers\src\*.cpp -- -I"SDL3-3.4.4/include" -I"checkers/include"
cls && clang-tidy checkers\src\checkers.cpp -checks=* --header-filter=ddd -- -I"SDL3-3.4.4/include" -I"checkers/include"
cls && clang++ -Wall -Wextra -o checkers\install\checkers.exe checkers\src\*.cpp -I"SDL3-3.4.4/include" -I"checkers/include" -l"SDL3-3.4.4/lib/x64/SDL3" -"Wl,/subsystem:console"
```
### SDL3>checkers
```
cls && cmake --build build --config Release && cmake --install build --config Release && cd install && .\checkers.exe && cd ..
```
