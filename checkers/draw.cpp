#include "draw.h"

#include <SDL3/SDL_rect.h>   //clang-tidy
#include <SDL3/SDL_render.h> //clang-tidy
#include <cmath>
// #include <SDL3/SDL_surface.h> //clang-tidy

auto draw_board(int xdim, int ydim, State *state) -> int { // NOLINT
  SDL_FRect dst_rect;
  for (int xpos = XPOS_START; xpos < (xdim - 1) * XPOS_STEP; // NOLINT
       xpos += XPOS_STEP) {
    for (int ypos = YPOS_START; ypos < (ydim - 1) * YPOS_STEP; // NOLINT
         ypos += YPOS_STEP) {
      dst_rect.x = static_cast<float>(xpos);
      dst_rect.y = static_cast<float>(ypos);
      dst_rect.w = static_cast<float>(state->blueortho->w) * SPRITE_SCALE;
      dst_rect.h = static_cast<float>(state->blueortho->h) * SPRITE_SCALE;
      SDL_RenderTexture(state->renderer, state->tblueortho, nullptr, &dst_rect);
    }
  }

  return 0;
}

auto draw_token(State *state) -> int { // NOLINT

  SDL_FRect dst_rect;

  dst_rect.w = static_cast<float>(state->blueortho->w) * SPRITE_SCALE / 2;
  dst_rect.h = static_cast<float>(state->blueortho->h) * SPRITE_SCALE / 2;

  if ((*state->pcylinder).x < static_cast<int>(round(WIDTH - XPOS_SPRITE_END)) &&
      (*state->pcylinder).y < static_cast<int>(round(HEIGHT - YPOS_SPRITE_END))) {
    (*state->pcylinder).x += 1;
    (*state->pcylinder).y += 1;
  }
  dst_rect.x = static_cast<float>(XPOS_SPRITE_OFFSET + (*state->pcylinder).x);
  dst_rect.y = static_cast<float>((*state->pcylinder).y);
  SDL_RenderTexture(state->renderer, state->tcylinder, nullptr, &dst_rect);

  return 0;
}
