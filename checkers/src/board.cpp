#include "board.h"
#include "common.h"          //clang-tidy
#include <SDL3/SDL_rect.h>   //clang-tidy
#include <SDL3/SDL_render.h> //clang-tidy

SDL_Point Board::max = {};
int Board::draw(int xdim, int ydim, State *state) { // NOLINT
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
      max.y = static_cast<int>(static_cast<float>(state->board->surf->h) *
                               SPRITE_SCALE) +
              ypos;
    }
    max.x = static_cast<int>(static_cast<float>(state->board->surf->w) *
                             SPRITE_SCALE) +
            xpos;
  }
  return 0;
}