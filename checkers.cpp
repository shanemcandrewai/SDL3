/*
  checkers.cpp

  cls && clang-tidy checkers.cpp -- -ISDL3-3.4.2\include ^
  -ISDL3_image-3.4.0\include

  cls && clang-tidy checkers.cpp -checks=* --header-filter=ddd --  ^
  -ISDL3-3.4.2\include -ISDL3_image-3.4.0\include 7808 warnings generated ^
  Suppressed 7814 warnings (7796 in non-user code, 18 NOLINT).

  clang -Wall -Wextra -o checkers.exe checkers.cpp -ISDL3-3.4.2\include ^
  -ISDL3_image-3.4.0\include -lSDL3-3.4.2\lib\x64\SDL3 ^
  -lSDL3_image-3.4.0\lib\x64\SDL3_image -Wl,/subsystem:console

  clang-format -i checkers.cpp

*/
#define SDL_MAIN_USE_CALLBACKS 1 // NOLINT
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
const float static SCALE = 2.0F;
static SDL_Surface *surface = NULL;
static SDL_Texture *texture = NULL;

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) { // NOLINT

  SDL_Window *window = nullptr;
  SDL_Renderer *renderer = nullptr;
  void *state[2];

  /* Create the window */
  if (!SDL_CreateWindowAndRenderer("Hello World", WIDTH, HEIGHT,
                                   SDL_WINDOW_RESIZABLE, &window, &renderer)) {
    SDL_Log("Couldn't create window and renderer: %s", // NOLINT
            SDL_GetError());                           // NOLINT
    return SDL_APP_FAILURE;
  }
  state[0] = renderer;
  state[1] = window;
  // *appstate = renderer;
  appstate = state;

  surface = SDL_LoadPNG("blender/blue.ortho.png");
  if (surface == nullptr) {
    SDL_Log("SDL_LoadPNG failed: %s", // NOLINT
            SDL_GetError());          // NOLINT
    return SDL_APP_FAILURE;
  }
  texture = SDL_CreateTextureFromSurface(renderer, surface);

  return SDL_APP_CONTINUE;
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) { // NOLINT
  if (event->type == SDL_EVENT_KEY_DOWN || event->type == SDL_EVENT_QUIT) {
    return SDL_APP_SUCCESS; /* end the program, reporting success to the OS. */
  }
  return SDL_APP_CONTINUE;
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate) { // NOLINT
  auto **state = static_cast<void **>(appstate);
  auto *local_renderer = static_cast<SDL_Renderer *>(state[0]);
  // auto *local_renderer = static_cast<SDL_Renderer *>(appstate);

  SDL_SetRenderDrawColor(local_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(local_renderer);

  SDL_FRect dst_rect;
  dst_rect.x = (100.0f);
  dst_rect.y = 0.0f;
  dst_rect.w = (float)surface->w;
  dst_rect.h = (float)surface->h;
  SDL_RenderTexture(local_renderer, texture, NULL, &dst_rect);

  int width = 0;
  int height = 0;

  /* Center the message and scale it up */
  SDL_GetRenderOutputSize(local_renderer, &width, &height);
  SDL_SetRenderScale(local_renderer, SCALE, SCALE);

  /* Draw the message */
  // SDL_SetRenderDrawColor(local_renderer, MAX, MAX, MAX, MAX);
  // SDL_RenderDebugText(local_renderer, xpos, ypos, message);
  SDL_RenderPresent(local_renderer);

  return SDL_APP_CONTINUE;
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result) {
} // NOLINT misc-use-anonymous-namespace
