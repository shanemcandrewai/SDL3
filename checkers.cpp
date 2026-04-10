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

const int static WIDTH = 1280;
const int static HEIGHT = 720;

struct State { // NOLINT altera-struct-pack-align
  SDL_Renderer *renderer;
  SDL_Surface *blueortho;
  SDL_Surface *cylinderpurp;
  SDL_Texture *tblueortho;
  SDL_Texture *tcylinderpurp;
};

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) { // NOLINT

  SDL_Window *window = nullptr;
  State * st = new State;

  if (!SDL_CreateWindowAndRenderer("Checkers test", WIDTH, HEIGHT,
                                   SDL_WINDOW_RESIZABLE, &window, &st->renderer)) {
    SDL_Log("Couldn't create window and renderer: %s", // NOLINT
            SDL_GetError());
    return SDL_APP_FAILURE;
  }

  // Enable VSync
  if (SDL_SetRenderVSync(st->renderer, 1) == false) {
    SDL_Log("Could not enable VSync! SDL error: %s\n", SDL_GetError());
  }

  st->blueortho = SDL_LoadPNG("assets/blue.ortho.png");
  if (st->blueortho == nullptr) {
    SDL_Log("SDL_LoadPNG failed: %s", // NOLINT
            SDL_GetError());
    return SDL_APP_FAILURE;
  }
  st->cylinderpurp = SDL_LoadPNG("assets/CylinderGold.png");
  if (st->cylinderpurp == nullptr) {
    SDL_Log("SDL_LoadPNG failed: %s", // NOLINT
            SDL_GetError());
    return SDL_APP_FAILURE;
  }

  if (!SDL_SetWindowIcon(window, st->cylinderpurp)) {
    SDL_Log("SDL_SetWindowIcon failed2: %s", // NOLINT
            SDL_GetError());
  }

  st->tblueortho = SDL_CreateTextureFromSurface(st->renderer, st->blueortho);
  st->tcylinderpurp = SDL_CreateTextureFromSurface(st->renderer, st->cylinderpurp);
  *appstate = st;
  
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
  auto *renderer = state->renderer;
  auto *sBlue = state->blueortho;
  auto *textBlue = state->tblueortho;
  auto *textCylinderPurp = state->tcylinderpurp;

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(renderer);

  SDL_FRect dst_rect;
  for (int xpos = 10; xpos < 900; xpos += 230) {
    for (int ypos = 10; ypos < 400; ypos += 105) {
      dst_rect.w = static_cast<float>(sBlue->w);
      dst_rect.h = static_cast<float>(sBlue->h);
      dst_rect.x = static_cast<float>(xpos);
      dst_rect.y = static_cast<float>(ypos);
      SDL_RenderTexture(renderer, textBlue, nullptr, &dst_rect);
      dst_rect.w = static_cast<float>(sBlue->w) / 2;
      dst_rect.h = static_cast<float>(sBlue->h) / 2;
      SDL_RenderTexture(renderer, textCylinderPurp, nullptr, &dst_rect);
    }
  }

  SDL_RenderPresent(renderer);

  return SDL_APP_CONTINUE;
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result) { // NOLINT
  delete static_cast<State *>(appstate);                 // NOLINT
} // NOLINT misc-use-anonymous-namespace
