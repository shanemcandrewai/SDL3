// checkers.cpp
#define SDL_MAIN_USE_CALLBACKS 1 // NOLINT cppcoreguidelines-macro-usage
#include <SDL3/SDL_main.h>    //keep uncommented for release

#include "draw.h"
// #include <SDL3/SDL_error.h>   //clang-tidy
// #include <SDL3/SDL_events.h>  //clang-tidy
// #include <SDL3/SDL_init.h>    //clang-tidy
// #include <SDL3/SDL_log.h>     //clang-tidy
// #include <SDL3/SDL_pixels.h>  //clang-tidy
// #include <SDL3/SDL_rect.h>    //clang-tidy
// #include <SDL3/SDL_render.h>  //clang-tidy
// #include <SDL3/SDL_surface.h> //clang-tidy
// #include <SDL3/SDL_video.h>   //clang-tidy


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

  if (draw_board(0, 0, state) > 0) {
    SDL_Log("draw board failed"); // NOLINT
  }

  SDL_FRect dst_rect;

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
