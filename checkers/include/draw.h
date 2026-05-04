#ifndef DRAW
#define DRAW
#include <SDL3/SDL.h>
#include <cmath>
#include <string>

// const std::string TOKEN_FILE = "assets/CylinderPurp.png";
const std::string TOKEN_FILE = "assets/CylinderGold.png";
const std::string BOARD_FILE = "assets/blue.ortho.png";
const std::string WINDOW_TITLE = "checkers";

const int XDIM = 3;
const int YDIM = 7;
const float SPEED_INIT = 1;
const float SPEED_MAX = 2;

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

typedef struct Point_float {
  float x;
  float y;
} Point_float;

typedef struct Board { // NOLINT altera-struct-pack-align
  SDL_Surface *surf;
  SDL_Texture *textu;
  int xdim;
  int ydim;
} Board;

typedef struct Token { // NOLINT altera-struct-pack-align
  SDL_Surface *surf;
  SDL_Texture *textu;
  Point_float *point;
  Point_float *step;
  SDL_Point *from;
  SDL_Point *to;
  float speed;
} Token;

struct State { // NOLINT altera-struct-pack-align
  SDL_Renderer *renderer;
  Board *board;
  Token *token;
};

extern auto calc_token_to(int, int, State *) -> int;
extern auto draw_board(int, int, State *) -> int;
extern auto draw_token(State *) -> int;
extern auto calc_point(State *) -> int;

#endif
