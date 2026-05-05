#ifndef BOARD_H
#define BOARD_H

#include "common.h"

class Board {
  public:
    static SDL_Point max;
    static int draw(int xdim, int ydim, State *state);
};
#endif
