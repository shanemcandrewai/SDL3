#ifndef DRAW
#define DRAW
#define SDL_MAIN_USE_CALLBACKS 1 // NOLINT cppcoreguidelines-macro-usage
// #include <SDL3/SDL.h>  //uncomment for release
#include <SDL3/SDL_error.h>   //clang-tidy
#include <SDL3/SDL_events.h>  //clang-tidy
#include <SDL3/SDL_init.h>    //clang-tidy
#include <SDL3/SDL_log.h>     //clang-tidy
#include <SDL3/SDL_main.h>    //keep uncommented for release
#include <SDL3/SDL_pixels.h>  //clang-tidy
#include <SDL3/SDL_rect.h>    //clang-tidy
#include <SDL3/SDL_render.h>  //clang-tidy
#include <SDL3/SDL_surface.h> //clang-tidy
#include <SDL3/SDL_video.h>   //clang-tidy

const float SPRITE_SCALE = 0.50;
const int WIDTH_FULL_HD = 1920;
const int HEIGHT_FULL_HD = 1080;
const int WIDTH_PIXEL8A = 1080;
const int HEIGHT_PIXEL8A = 2400;

#ifndef __EMSCRIPTEN__
const int WIDTH = static_cast<int>(round(WIDTH_FULL_HD * SPRITE_SCALE));
const int HEIGHT = static_cast<int>(round(HEIGHT_FULL_HD * SPRITE_SCALE));
const float BOARD_SCALE_X = 0.5;
#else
const int WIDTH = static_cast<int>(round(HEIGHT_PIXEL8A * SPRITE_SCALE));
const int HEIGHT = static_cast<int>(round(WIDTH_PIXEL8A * SPRITE_SCALE));
const float BOARD_SCALE_X = 0.6;
#endif
const float BOARD_SCALE_Y = 0.4;

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

extern int draw_board(int, int, State *);

#endif