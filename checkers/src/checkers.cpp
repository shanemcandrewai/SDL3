// checkers.cpp
#define SDL_MAIN_USE_CALLBACKS 1 // NOLINT cppcoreguidelines-macro-usage
#include <SDL3/SDL_main.h>       //keep uncommented for release
#include <cmath>

#include "board.h"
#include "common.h"
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

  auto *state = new State;   // NOLINT cppcoreguidelines-owning-memory
  state->token = new Token;  // NOLINT cppcoreguidelines-owning-memory
  state->board = new SBoard; // NOLINT cppcoreguidelines-owning-memory
  state->board->xdim = XDIM;
  state->board->ydim = YDIM;
  state->token->point = new Point_float; // NOLINT
  state->token->step = new Point_float;  // NOLINT
  state->token->from = new SDL_Point; // NOLINT cppcoreguidelines-owning-memory
  state->token->to = new SDL_Point;   // NOLINT cppcoreguidelines-owning-memory
  state->token->speed = SPEED_INIT;
  state->token->from->x = XPOS_START;
  state->token->from->y = YPOS_START;

#ifdef __EMSCRIPTEN__
  const SDL_WindowFlags window_flags = SDL_WINDOW_FILL_DOCUMENT;
#else
  const SDL_WindowFlags window_flags = SDL_WINDOW_RESIZABLE;
#endif

  if (!SDL_CreateWindowAndRenderer(WINDOW_TITLE.c_str(), WIDTH, HEIGHT,
                                   window_flags, &window, &state->renderer)) {
    SDL_Log("Couldn't create window and renderer: %s", // NOLINT
            SDL_GetError());
    return SDL_APP_FAILURE;
  }
  // SDL_Log("rend: %s\n", SDL_GetRendererName(state->renderer));

#ifndef __EMSCRIPTEN__
  if (!SDL_SetRenderVSync(state->renderer, 1)) {
    SDL_Log("SDL_SetRenderVSync failed: %s", // NOLINT
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

  state->token->point->x = static_cast<float>(XPOS_START);
  state->token->point->y = static_cast<float>(YPOS_START) / 3;
  if (calc_token_to(state->board->xdim - 1, state->board->ydim - 1, state) >
      0) {
    SDL_Log("calc_token_to failed"); // NOLINT
  }

  *appstate = state;
  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) { // NOLINT
  auto *state = static_cast<State *>(appstate);

  SDL_SetRenderDrawColor(state->renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(state->renderer);

  if (Board::draw(state->board->xdim, state->board->ydim, state) > 0) {
    SDL_Log("draw board failed"); // NOLINT
  }

  if (draw_token(state) > 0) {
    SDL_Log("draw token failed"); // NOLINT
  }

  SDL_RenderPresent(state->renderer);

  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) { // NOLINT
  auto *state = static_cast<State *>(appstate);
  int width = 0;
  int height = 0;
  SDL_GetRenderOutputSize(state->renderer, &width, &height);

  switch (event->type) {
  case SDL_EVENT_KEY_DOWN:
  case SDL_EVENT_QUIT:
    return SDL_APP_SUCCESS;
  case SDL_EVENT_FINGER_DOWN: {
    // const int touch_x = static_cast<int>(std::round(event->tfinger.x * XDIM / static_cast<float>(Board::max.x)));
    // const int touch_y = static_cast<int>(std::round(event->tfinger.y * YDIM / static_cast<float>(Board::max.y)));
    const int touch_x = static_cast<int>(std::round(event->tfinger.x * XDIM / static_cast<float>(width)));
    const int touch_y = static_cast<int>(std::round(event->tfinger.y * YDIM / static_cast<float>(height)));
    calc_token_to(touch_x, touch_y, state);
    SDL_Log("\n");                                    // NOLINT
    SDL_Log("touch_x %d\n", state->token->to->x); // NOLINT
    SDL_Log("touch_y %d\n", state->token->to->y); // NOLINT
    break;
  }
  case SDL_EVENT_MOUSE_BUTTON_DOWN: {
    const int mouse_x = static_cast<int>(std::round(event->button.x * XDIM / static_cast<float>(Board::max.x)));
    const int mouse_y = static_cast<int>(std::round(event->button.y * YDIM / static_cast<float>(Board::max.y)));
    calc_token_to(mouse_x, mouse_y, state);
    SDL_Log("\n");                                    // NOLINT
    SDL_Log("mouse_x %d\n", state->token->to->x); // NOLINT
    SDL_Log("mouse_y %d\n", state->token->to->y); // NOLINT
    break;
  }
  default:
    break;
  }
  return SDL_APP_CONTINUE;
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result) { // NOLINT
  delete static_cast<State *>(appstate);                 // NOLINT
} // NOLINT misc-use-anonymous-namespace
