#include "draw.h"
#include <random>

#include <SDL3/SDL_rect.h>   //clang-tidy
#include <SDL3/SDL_render.h> //clang-tidy
#include <SDL3/SDL_stdinc.h> //clang-tidy

auto draw_board(int xdim, int ydim, State *state) -> int { // NOLINT
  SDL_FRect dst_rect;
  for (int xpos = XPOS_START; xpos < (xdim - 1) * XPOS_STEP; // NOLINT
       xpos += XPOS_STEP) {

    for (int ypos = YPOS_START; ypos < (ydim - 1) * YPOS_STEP; // NOLINT
         ypos += YPOS_STEP) {

      dst_rect.x = static_cast<float>(xpos);
      dst_rect.y = static_cast<float>(ypos);
      dst_rect.w = static_cast<float>(state->board->surf->w) * SPRITE_SCALE;
      dst_rect.h = static_cast<float>(state->board->surf->h) * SPRITE_SCALE;
      SDL_RenderTexture(state->renderer, state->board->textu, nullptr,
                        &dst_rect);
    }
  }
  return 0;
}

auto draw_token(State *state) -> int { // NOLINT

  SDL_FRect dst_rect;

  dst_rect.w = static_cast<float>(state->board->surf->w) * SPRITE_SCALE / 2;
  dst_rect.h = static_cast<float>(state->board->surf->h) * SPRITE_SCALE / 2;

  if (calc_point(state) > 0) {
    SDL_Log("calc_point failed"); // NOLINT
  }

  dst_rect.x = static_cast<float>(XPOS_SPRITE_OFFSET) + state->token->point->x;
  dst_rect.y = static_cast<float>(XPOS_SPRITE_OFFSET) + state->token->point->y;
  SDL_RenderTexture(state->renderer, state->token->textu, nullptr, &dst_rect);
  return 0;
}

auto calc_point(State *state) -> int { // NOLINT
  bool step_executed = false;
  if (state->token->step->x > 0 &&
      state->token->point->x < static_cast<float>(state->token->to->x)) {
    const float orig = state->token->point->x;
    state->token->point->x += state->token->step->x;
    if (orig != state->token->point->x) {
      step_executed = true;
    }
  }
  if (state->token->step->x < 0 &&
      state->token->point->x > static_cast<float>(state->token->to->x)) {
    const float orig = state->token->point->x;
    state->token->point->x += state->token->step->x;
    if (orig != state->token->point->x) {
      step_executed = true;
    }
  }
  if (state->token->step->y > 0 &&
      state->token->point->y < static_cast<float>(state->token->to->y)) {
    const float orig = state->token->point->y;
    state->token->point->y += state->token->step->y;
    if (orig != state->token->point->y) {
      step_executed = true;
    }
  }
  if (state->token->step->y < 0 &&
      state->token->point->y > static_cast<float>(state->token->to->y)) {
    const float orig = state->token->point->y;
    state->token->point->y += state->token->step->y;
    if (orig != state->token->point->y) {
      step_executed = true;
    }
  }

  if (!step_executed) {
    std::random_device rdev;
    std::mt19937 merst(rdev());
    std::uniform_int_distribution<int> distx(0, state->board->xdim - 1);
    std::uniform_int_distribution<int> disty(0, state->board->ydim - 1);

    if (calc_token_to(distx(merst), disty(merst), state) > 0) {
      SDL_Log("calc_token_to failed"); // NOLINT
      return 1;
    }
  }
  return 0;
}

auto calc_token_to(int xdim, int ydim, State *state) -> int { // NOLINT
  state->token->to->x = (XPOS_STEP * xdim) + XPOS_START;
  state->token->to->y = (YPOS_STEP * ydim) + YPOS_START;
  const double angle = SDL_atan2(
      static_cast<float>(state->token->to->y) - state->token->point->y,
      static_cast<float>(state->token->to->x) - state->token->point->x);

  state->token->step->x = static_cast<float>(SDL_cos(angle));
  state->token->step->y = static_cast<float>(SDL_sin(angle));
  return 0;
}
