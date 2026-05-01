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


int draw_board(int xdim, int ydim, State *state) {
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

  return 0;
}