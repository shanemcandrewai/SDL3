#include "draw.h"

#include <SDL3/SDL_rect.h>   //clang-tidy
#include <SDL3/SDL_render.h> //clang-tidy
// #include <SDL3/SDL_surface.h> //clang-tidy

auto draw_board(int xdim, int ydim, State *state) -> int { // NOLINT
  SDL_FRect dst_rect;
  for (int xpos = XPOS_START; xpos < (xdim - 1) * XPOS_STEP; // NOLINT
       xpos += XPOS_STEP) {
    for (int ypos = YPOS_START;         // NOLINT altera-unroll-loops
         ypos < (ydim - 1) * YPOS_STEP; // NOLINT
         ypos += YPOS_STEP) {           // NOLINT
      dst_rect.x = static_cast<float>(xpos);
      dst_rect.y = static_cast<float>(ypos);
      dst_rect.w = static_cast<float>(state->blueortho->w) * SPRITE_SCALE;
      dst_rect.h = static_cast<float>(state->blueortho->h) * SPRITE_SCALE;
      SDL_RenderTexture(state->renderer, state->tblueortho, nullptr, &dst_rect);
    }
  }

  return 0;
}