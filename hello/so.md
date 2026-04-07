I have two CMake projects which share the same SDL3 Hello, World! C++ source file ```hello.cpp```. Individually, I can build and test them successfully and now I want to create a master ```CMakeLists.txt``` to build them both.

Directory structure:

```lang-plaintext
hello/
hello/hello.cpp
hello/CMakeLists.txt (the master CMakesList.txt which currently does not work)
hello/hello_clang/
hello/hello_clang/CMakeLists.txt (builds hello_clange.exe correctly)
hello/hello_clang/SDL3-3.4.2 (https://github.com/libsdl-org/SDL/releases/download/release-3.4.2/SDL3-devel-3.4.2-VC.zip)
hello/hello_clang/SDL3.dll  (https://github.com/libsdl-org/SDL/releases/download/release-3.4.2/SDL3-devel-3.4.2-VC.zip)
hello/hello_emscripten/
hello/hello_emscripten/CMakeLists.txt (builds hello_emscripten.js and hello_emscripten.wasm correctly)
hello/hello_emscripten/index.html (for testing hello_emscripten.js and hello_emscripten.wasm)
hello/hello_emscripten/emsdk/ (git clone https://github.com/emscripten-core/emsdk; updated according to https://emscripten.org/docs/getting_started/downloads.html#updating-the-sdk)
hello/hello_emscripten/install_libSDL3/ (SDL3 compiled to emscripten)
```

```hello.cpp```:

```lang-cpp
/*
  SDL3 hello.cpp
  adapted from https://github.com/libsdl-org/SDL/blob/main/docs/hello.c
  cls && clang-tidy hello.c -checks=* --header-filter=ddd --
  -ISDL3-3.4.2\include

  Copyright (C) 1997-2026 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely.

*/
#define SDL_MAIN_USE_CALLBACKS 1 /* use the callbacks instead of main() */ //NOLINT
// #include <SDL3/SDL.h>  //uncomment for release
#include <SDL3/SDL_error.h>  //clang-tidy
#include <SDL3/SDL_events.h> //clang-tidy
#include <SDL3/SDL_init.h>   //clang-tidy
#include <SDL3/SDL_log.h>    //clang-tidy
#include <SDL3/SDL_main.h>   //keep uncommented for release
#include <SDL3/SDL_render.h> //clang-tidy
#include <SDL3/SDL_stdinc.h> //clang-tidy
#include <SDL3/SDL_video.h>  //clang-tidy

const int static WIDTH = 800;
const int static HEIGHT = 800;
const int static MAX = 255;
const float static SCALE = 4.0F;
const char static *MESSAGE = "Hello, World!"; //NOLINT

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) { //NOLINT

  SDL_Window *window = nullptr;
  SDL_Renderer *renderer = nullptr;

  /* Create the window */
  if (!SDL_CreateWindowAndRenderer("Hello, World!", WIDTH, HEIGHT,
                                   SDL_WINDOW_FULLSCREEN, &window, &renderer)) {
    SDL_Log("Couldn't create window and renderer: %s", SDL_GetError()); //NOLINT
    return SDL_APP_FAILURE;
  }
  *appstate = renderer;
  return SDL_APP_CONTINUE;
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) { //NOLINT
  if (event->type == SDL_EVENT_KEY_DOWN || event->type == SDL_EVENT_QUIT) {
    return SDL_APP_SUCCESS; /* end the program, reporting success to the OS. */
  }
  return SDL_APP_CONTINUE;
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate) { //NOLINT
  auto *renderer = static_cast<SDL_Renderer *>(appstate);
  int width = 0;
  int height = 0;

  /* Center the message and scale it up */
  SDL_GetRenderOutputSize(renderer, &width, &height);
  SDL_SetRenderScale(renderer, SCALE, SCALE);
  const float xpos = ((static_cast<float>(width) / SCALE) -
                (static_cast<float>(SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE) *
                 SDL_strlen(MESSAGE))) /
               2;
  const float ypos = ((static_cast<float>(height) / SCALE) -
                SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE) /
               2;

  /* Draw the message */
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, MAX);
  SDL_RenderClear(renderer);
  SDL_SetRenderDrawColor(renderer, MAX, MAX, MAX, MAX);
  SDL_RenderDebugText(renderer, xpos, ypos, MESSAGE);
  SDL_RenderPresent(renderer);

  return SDL_APP_CONTINUE;
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result) {}//NOLINT misc-use-anonymous-namespace
```

```hello/hello_clang/CMakeLists.txt``` (builds hello_clange.exe correctly):

```
#[[

# build with shared prebuilt SDL3 library (DLL)

cmake -B build -G Ninja
cmake --build build

# test 
./build/hello_clang.exe

]]

cmake_minimum_required(VERSION 3.25)

project(hello_clang)

set(CMAKE_C_COMPILER "C:/Program Files/LLVM/bin/clang.exe")
set(CMAKE_CXX_COMPILER "C:/Program Files/LLVM/bin/clang++.exe")

add_library(SDL3 SHARED IMPORTED)
set_target_properties(SDL3 PROPERTIES
INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_LIST_DIR}/SDL3-3.4.2/include"
IMPORTED_IMPLIB "${CMAKE_CURRENT_LIST_DIR}/SDL3-3.4.2/lib/x64/SDL3.lib" # SHARED
)

add_executable(${PROJECT_NAME} "${CMAKE_CURRENT_LIST_DIR}/../hello.cpp")
target_link_libraries(${PROJECT_NAME} SDL3)
```

``` hello/hello_emscripten/CMakeLists.txt``` (builds hello_emscripten.js and hello_emscripten.wasm correctly):

```
#[[

# build
cmake -B build -G Ninja
cmake --build build

# test
python -m http.server 8000
http://localhost:8000/

]]
cmake_minimum_required(VERSION 3.25)


set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_LIST_DIR}/emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake")
project(hello_emscripten)

#set(CMAKE_BUILD_TYPE MinSizeRel)

# Set the name of the future application (in Windows this would be app.exe, while for web it will be app.js / app.wasm)
add_executable(${PROJECT_NAME} "${CMAKE_CURRENT_LIST_DIR}/../hello.cpp")

# Tell CMake where to find the SDL3 library configuration files
set(SDL3_DIR "${CMAKE_CURRENT_LIST_DIR}/install_libSDL3/lib/cmake/SDL3")

# Load SDL3 package settings (compilation parameters and paths to headers)
find_package(SDL3 REQUIRED)

# Link SDL3 to our application (configures linking and include paths)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)
```

```hello/hello_emscripten/index.html``` (for testing hello_emscripten.js and hello_emscripten.wasm):

```lang-html
<!DOCTYPE html>
<html lang="en">
   <head>
      <meta charset="UTF-8">
      <meta name="viewport" content="width=device-width,initial-scale=1">
      <title>SDL3 emscripten</title>
      <script async type="text/javascript" src="build/hello_emscripten.js"></script>
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
```

```hello/CMakeLists.txt``` (the master CMakesList.txt which currently does not work)

```
#[[

# build all

cmake -B build -G Ninja
cmake --build build

# test hello_clang
.\hello_clang\build\hello_clang.exe

# testing hello_emscripten
cd hello_emscripten
python -m http.server 8000
http://localhost:8000/

]]

cmake_minimum_required(VERSION 3.25)

project(hello_all)

add_subdirectory(hello_clang)
add_subdirectory(hello_emscripten)
```

When I execute the above with

```lang-bash
cmake -B build -G Ninja
```

I get the following error:

```lang-plaintext
CMake Error at hello_emscripten/CMakeLists.txt:28 (find_package):
  Could not find a configuration file for package "SDL3" that is compatible
  with requested version "".

  The following configuration files were considered but not accepted:

    C:/Users/xxx/dev/SDL3/hello/hello_emscripten/install_libSDL3/lib/cmake/SDL3/SDL3Config.cmake, version: 3.4.4 (32bit)
      The version found is not compatible with the version requested.
```

What is the problem?