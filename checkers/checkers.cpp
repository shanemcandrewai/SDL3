// checkers.cpp
#define SDL_MAIN_USE_CALLBACKS 1 // NOLINT cppcoreguidelines-macro-usage
#include <SDL3/SDL_main.h>       //keep uncommented for release

#include "draw.h"
#include <SDL3/SDL_error.h>   //clang-tidy
#include <SDL3/SDL_events.h>  //clang-tidy
#include <SDL3/SDL_init.h>    //clang-tidy
#include <SDL3/SDL_log.h>     //clang-tidy
#include <SDL3/SDL_pixels.h>  //clang-tidy
#include <SDL3/SDL_render.h>  //clang-tidy
#include <SDL3/SDL_surface.h> //clang-tidy
#include <SDL3/SDL_video.h>   //clang-tidy


SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) { // NOLINT
  SDL_Window *window = nullptr;

  auto *state = new State;          // NOLINT cppcoreguidelines-owning-memory
  state->token = new Token; // NOLINT cppcoreguidelines-owning-memory
  state->board = new Board; // NOLINT cppcoreguidelines-owning-memory
  state->board->xdim = XDIM;
  state->board->ydim = YDIM;
  state->token->point = new SDL_Point; // NOLINT cppcoreguidelines-owning-memory
  state->token->from = new SDL_Point; // NOLINT cppcoreguidelines-owning-memory
  state->token->to = new SDL_Point; // NOLINT cppcoreguidelines-owning-memory
  state->token->speed = SPEED_INIT;

#ifndef __EMSCRIPTEN__
  if (!SDL_CreateWindowAndRenderer(WINDOW_TITLE.c_str(), WIDTH, HEIGHT,
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
  if (!SDL_CreateWindowAndRenderer(WINDOW_TITLE.c_str(), WIDTH, HEIGHT,
                                   SDL_WINDOW_FILL_DOCUMENT,
                                   //  SDL_WINDOW_FULLSCREEN,
                                   &window, &state->renderer)) {
    SDL_Log("Couldn't create window and renderer: %s", // NOLINT
            SDL_GetError());
    return SDL_APP_FAILURE;
  }
#endif

  state->board->surf = SDL_LoadPNG(BOARD_FILE.c_str());
  if (state->board->surf == nullptr) {
    SDL_Log("SDL_LoadPNG failed: %s", // NOLINT hicpp-vararg
            SDL_GetError());
    return SDL_APP_FAILURE;
  }
  state->token->surf = SDL_LoadPNG(TOKEN_FILE.c_str());
  if (state->token->surf == nullptr) {
    SDL_Log("SDL_LoadPNG failed: %s", // NOLINT hicpp-vararg
            SDL_GetError());
    return SDL_APP_FAILURE;
  }

  if (!SDL_SetWindowIcon(window, state->token->surf)) {
    SDL_Log("SDL_SetWindowIcon failed: %s", // NOLINT hicpp-vararg
            SDL_GetError());
  }

  state->board->textu =
      SDL_CreateTextureFromSurface(state->renderer, state->board->surf);
  state->token->textu =
      SDL_CreateTextureFromSurface(state->renderer, state->token->surf);

  state->token->point->x = XPOS_START;
  state->token->point->y = YPOS_START / 3;
  *appstate = state;

  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) { // NOLINT
  auto *state = static_cast<State *>(appstate);

  SDL_SetRenderDrawColor(state->renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(state->renderer);

  if (draw_board(state->board->xdim, state->board->ydim, state) > 0) {
    SDL_Log("draw board failed"); // NOLINT
  }

  if (draw_token(state) > 0) {
    SDL_Log("draw board failed"); // NOLINT
  }

  SDL_RenderPresent(state->renderer);

  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) { // NOLINT
  if (event->type == SDL_EVENT_KEY_DOWN || event->type == SDL_EVENT_QUIT) {
    return SDL_APP_SUCCESS;
  }
  return SDL_APP_CONTINUE;
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result) { // NOLINT
  delete static_cast<State *>(appstate);                 // NOLINT
} // NOLINT misc-use-anonymous-namespace
