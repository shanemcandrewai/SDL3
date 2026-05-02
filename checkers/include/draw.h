#ifndef DRAW
#define DRAW
#include <SDL3/SDL.h>
#include <cmath>

const float SPRITE_SCALE = 0.50;
const int WIDTH_FULL_HD = 1920;
const int HEIGHT_FULL_HD = 1080;
const int WIDTH_PIXEL8A = 1080;
const int HEIGHT_PIXEL8A = 2400;

#ifndef __EMSCRIPTEN__
const int WIDTH = static_cast<int>(round(WIDTH_FULL_HD * SPRITE_SCALE));
const int HEIGHT = static_cast<int>(round(HEIGHT_FULL_HD * SPRITE_SCALE));
#else
const int WIDTH = static_cast<int>(round(HEIGHT_PIXEL8A * SPRITE_SCALE));
const int HEIGHT = static_cast<int>(round(WIDTH_PIXEL8A * SPRITE_SCALE));
#endif

const int XPOS_START = static_cast<int>(round(-20 * SPRITE_SCALE));
const int XPOS_SPRITE_OFFSET = static_cast<int>(round(70 * SPRITE_SCALE));
const int XPOS_SPRITE_END = static_cast<int>(round(110 * SPRITE_SCALE));
const int YPOS_SPRITE_END = static_cast<int>(round(70 * SPRITE_SCALE));
const int YPOS_START = static_cast<int>(round(-70 * SPRITE_SCALE));
const int XPOS_STEP = static_cast<int>(round(230 * SPRITE_SCALE));
const int YPOS_STEP = static_cast<int>(round(140 * SPRITE_SCALE));
const int TOKEN_OFFSET = static_cast<int>(round(40 * SPRITE_SCALE));

struct State { // NOLINT altera-struct-pack-align
  SDL_Renderer *renderer;
  SDL_Surface *blueortho;
  SDL_Surface *scylinder;
  SDL_Texture *tblueortho;
  SDL_Texture *tcylinder;
  int xpos;
  int ypos;
};

extern auto draw_board(int, int, State *) -> int;

#endif