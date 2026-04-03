/*
  checkers.cpp
*/
#define SDL_MAIN_USE_CALLBACKS 1 // NOLINT
// #include <SDL3/SDL.h>  //uncomment for release
#include <SDL3/SDL_error.h>   //clang-tidy
#include <SDL3/SDL_events.h>  //clang-tidy
#include <SDL3/SDL_init.h>    //clang-tidy
#include <SDL3/SDL_log.h>     //clang-tidy
#include <SDL3/SDL_main.h>    //keep uncommented for release
#include <SDL3/SDL_pixels.h>  //clang-tidy
#include <SDL3/SDL_rect.h>    //clang-tidy
#include <SDL3/SDL_render.h>  //clang-tidy
#include <SDL3/SDL_surface.h> //clang-tidy
#include <SDL3/SDL_video.h>   //clang-tidy

const int static WIDTH = 600;
const int static HEIGHT = 600;

struct State { // NOLINT altera-struct-pack-align
  SDL_Renderer *prenderer;
  SDL_Surface *psurface;
  SDL_Texture *ptexture;
};

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) { // NOLINT

  SDL_Window *window = nullptr;
  SDL_Renderer *renderer = nullptr;
  SDL_Surface *surface = nullptr;
  SDL_Texture *texture = nullptr;

  /* Create the window */
  if (!SDL_CreateWindowAndRenderer("Hello World", WIDTH, HEIGHT,
                                   SDL_WINDOW_RESIZABLE, &window, &renderer)) {
    SDL_Log("Couldn't create window and renderer: %s", // NOLINT
            SDL_GetError());
    return SDL_APP_FAILURE;
  }

  surface = SDL_LoadPNG("blender/blue.ortho.png");
  if (surface == nullptr) {
    SDL_Log("SDL_LoadPNG failed: %s", // NOLINT
            SDL_GetError());
    return SDL_APP_FAILURE;
  }
  texture = SDL_CreateTextureFromSurface(renderer, surface);
  *appstate = new State{renderer, surface, texture}; // NOLINT
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
  auto *state = static_cast<State *>(appstate);
  auto *renderer = state->prenderer;
  auto *surface = state->psurface;
  auto *texture = state->ptexture;

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(renderer);

  SDL_FRect dst_rect;
  dst_rect.w = static_cast<float>(surface->w);
  dst_rect.h = static_cast<float>(surface->h);
  for (int i = 10; i < 400; i += 100) {
    // SDL_Log("i: %d", i);// NOLINT
    dst_rect.x =  static_cast<float>(i);
    dst_rect.y = static_cast<float>(i);
    SDL_RenderTexture(renderer, texture, nullptr, &dst_rect);
  }

  SDL_RenderPresent(renderer);

  return SDL_APP_CONTINUE;
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result) { // NOLINT
  delete static_cast<State *>(appstate);                 // NOLINT
} // NOLINT misc-use-anonymous-namespace
