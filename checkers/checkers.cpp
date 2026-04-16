// checkers.cpp
#define SDL_MAIN_USE_CALLBACKS 1 // NOLINT cppcoreguidelines-macro-usage
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
#include <cmath>

const float static SPRITE_SCALE = 0.50;
const int static WIDTH_FULL_HD = 1920;
const int static HEIGHT_FULL_HD = 1080;
const int static WIDTH_PIXEL8A = 1080;
const int static HEIGHT_PIXEL8A = 2400;

#ifndef __EMSCRIPTEN__
const int static WIDTH = static_cast<int>(round(WIDTH_FULL_HD * SPRITE_SCALE));
const int static HEIGHT =
    static_cast<int>(round(HEIGHT_FULL_HD * SPRITE_SCALE));
const float static BOARD_SCALE_X = 0.5;
#else
const int static WIDTH = static_cast<int>(round(HEIGHT_PIXEL8A * SPRITE_SCALE));
const int static HEIGHT = static_cast<int>(round(WIDTH_PIXEL8A * SPRITE_SCALE));
const float static BOARD_SCALE_X = 0.6;
#endif
const float static BOARD_SCALE_Y = 0.4;

const int static XPOS_START = static_cast<int>(round(-20 * SPRITE_SCALE));
const int static XPOS_SPRITE_OFFSET =
    static_cast<int>(round(70 * SPRITE_SCALE));
const int static XPOS_SPRITE_END = static_cast<int>(round(110 * SPRITE_SCALE));
const int static YPOS_SPRITE_END = static_cast<int>(round(70 * SPRITE_SCALE));
const int static YPOS_START = static_cast<int>(round(-70 * SPRITE_SCALE));
const int static XPOS_STEP = static_cast<int>(round(230 * SPRITE_SCALE));
const int static YPOS_STEP = static_cast<int>(round(140 * SPRITE_SCALE));
const int static TOKEN_OFFSET = static_cast<int>(round(40 * SPRITE_SCALE));

struct State { // NOLINT altera-struct-pack-align
  SDL_Renderer *renderer;
  SDL_Surface *blueortho;
  SDL_Surface *scylinder;
  SDL_Texture *tblueortho;
  SDL_Texture *tcylinder;
  int xpos;
  int ypos;
};

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) { // NOLINT
  SDL_Window *window = nullptr;
  auto *state = new State; // NOLINT cppcoreguidelines-owning-memory

#ifndef __EMSCRIPTEN__
  if (!SDL_CreateWindowAndRenderer("Checkers", WIDTH, HEIGHT,
                                   SDL_WINDOW_RESIZABLE, &window,
                                   &state->renderer)) {
    SDL_Log("Couldn't create window and renderer: %s", // NOLINT
            SDL_GetError());
    return SDL_APP_FAILURE;
  }
  if (!SDL_SetRenderVSync(state->renderer, 1)) {
    SDL_Log("Could not enable VSync! SDL error: %s", // NOLINT
            SDL_GetError());
  }
#else
  if (!SDL_CreateWindowAndRenderer("Checkers", WIDTH, HEIGHT,
                                   SDL_WINDOW_FILL_DOCUMENT,
                                      //  SDL_WINDOW_FULLSCREEN,
                                   &window, &state->renderer)) {
    SDL_Log("Couldn't create window and renderer: %s", // NOLINT
            SDL_GetError());
    return SDL_APP_FAILURE;
  }
#endif

  state->blueortho = SDL_LoadPNG("assets/blue.ortho.png");
  if (state->blueortho == nullptr) {
    SDL_Log("SDL_LoadPNG failed: %s", // NOLINT hicpp-vararg
            SDL_GetError());
    return SDL_APP_FAILURE;
  }
  state->scylinder = SDL_LoadPNG("assets/CylinderGold.png");
  if (state->scylinder == nullptr) {
    SDL_Log("SDL_LoadPNG failed: %s", // NOLINT hicpp-vararg
            SDL_GetError());
    return SDL_APP_FAILURE;
  }

  if (!SDL_SetWindowIcon(window, state->scylinder)) {
    SDL_Log("SDL_SetWindowIcon failed: %s", // NOLINT hicpp-vararg
            SDL_GetError());
  }

  state->tblueortho =
      SDL_CreateTextureFromSurface(state->renderer, state->blueortho);
  state->tcylinder =
      SDL_CreateTextureFromSurface(state->renderer, state->scylinder);

  (*state).xpos = XPOS_START;
  (*state).ypos = YPOS_START / 3;
  *appstate = state;

  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) { // NOLINT
  if (event->type == SDL_EVENT_KEY_DOWN || event->type == SDL_EVENT_QUIT) {
    return SDL_APP_SUCCESS;
  }
  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) { // NOLINT
  auto *state = static_cast<State *>(appstate);

  SDL_SetRenderDrawColor(state->renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(state->renderer);

  SDL_FRect dst_rect;
  for (int xpos = XPOS_START;
       xpos < static_cast<int>(round(WIDTH * BOARD_SCALE_X)); // NOLINT
       xpos += XPOS_STEP) {
    for (int ypos = YPOS_START; // NOLINT altera-unroll-loops
         ypos < static_cast<int>(round(HEIGHT * BOARD_SCALE_Y)); // NOLINT
         ypos += YPOS_STEP) {                                    // NOLINT
      dst_rect.x = static_cast<float>(xpos);
      dst_rect.y = static_cast<float>(ypos);
      dst_rect.w = static_cast<float>(state->blueortho->w) * SPRITE_SCALE;
      dst_rect.h = static_cast<float>(state->blueortho->h) * SPRITE_SCALE;
      SDL_RenderTexture(state->renderer, state->tblueortho, nullptr, &dst_rect);
    }
  }

  dst_rect.w = static_cast<float>(state->blueortho->w) * SPRITE_SCALE / 2;
  dst_rect.h = static_cast<float>(state->blueortho->h) * SPRITE_SCALE / 2;

  if ((*state).xpos <
          static_cast<int>(round((WIDTH - XPOS_SPRITE_END) * // NOLINT
                                 BOARD_SCALE_X)) &&          // NOLINT
      (*state).ypos <
          static_cast<int>(round((HEIGHT - YPOS_SPRITE_END) * // NOLINT
                                 BOARD_SCALE_Y))) {           // NOLINT
    (*state).xpos += 1;
    (*state).ypos += 1;
  }
  dst_rect.x = static_cast<float>(XPOS_SPRITE_OFFSET + (*state).xpos);
  dst_rect.y = static_cast<float>((*state).ypos);
  SDL_RenderTexture(state->renderer, state->tcylinder, nullptr, &dst_rect);

  SDL_RenderPresent(state->renderer);

  return SDL_APP_CONTINUE;
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result) { // NOLINT
  delete static_cast<State *>(appstate);                 // NOLINT
} // NOLINT misc-use-anonymous-namespace
