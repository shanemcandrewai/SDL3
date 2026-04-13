// checkers.cpp
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

const int static WIDTH = 1920;
const int static HEIGHT = 1080;

struct State { // NOLINT altera-struct-pack-align
  SDL_Renderer *renderer;
  SDL_Surface *blueortho;
  SDL_Surface *cylinderpurp;
  SDL_Texture *tblueortho;
  SDL_Texture *tcylinderpurp;
};

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) { // NOLINT
  SDL_LogInfo(SDL_LOG_CATEGORY_TEST, "checkers test 1");
  SDL_Window *window = nullptr;
  auto *state = new State; // NOLINT cppcoreguidelines-owning-memory

  if (!SDL_CreateWindowAndRenderer("Checkers", WIDTH, HEIGHT,
                                   SDL_WINDOW_FILL_DOCUMENT, &window,
                                   // SDL_WINDOW_FULLSCREEN, &window,
                                   &state->renderer)) {
    SDL_Log("Couldn't create window and renderer: %s", // NOLINT
            SDL_GetError());
    return SDL_APP_FAILURE;
  }

  // Enable VSync
  if (!SDL_SetRenderVSync(state->renderer, 1)) {
    SDL_Log("Could not enable VSync! SDL error: %s\n", // NOLINT
            SDL_GetError());
  }

  state->blueortho = SDL_LoadPNG("assets/blue.ortho.png");
  if (state->blueortho == nullptr) {
    SDL_Log("SDL_LoadPNG failed: %s", // NOLINT hicpp-vararg
            SDL_GetError());
    return SDL_APP_FAILURE;
  }
  state->cylinderpurp = SDL_LoadPNG("assets/CylinderGold.png");
  if (state->cylinderpurp == nullptr) {
    SDL_Log("SDL_LoadPNG failed: %s", // NOLINT hicpp-vararg
            SDL_GetError());
    return SDL_APP_FAILURE;
  }

  if (!SDL_SetWindowIcon(window, state->cylinderpurp)) {
    SDL_Log("SDL_SetWindowIcon failed2: %s", // NOLINT hicpp-vararg
            SDL_GetError());
  }

  state->tblueortho =
      SDL_CreateTextureFromSurface(state->renderer, state->blueortho);
  state->tcylinderpurp =
      SDL_CreateTextureFromSurface(state->renderer, state->cylinderpurp);
  *appstate = state;

  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) { // NOLINT
  if (event->type == SDL_EVENT_KEY_DOWN || event->type == SDL_EVENT_QUIT) {
    return SDL_APP_SUCCESS; /* end the program, reporting success to the OS. */
  }
  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) { // NOLINT
  auto *state = static_cast<State *>(appstate);
  SDL_SetRenderDrawColor(state->renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(state->renderer);

  SDL_FRect dst_rect;
  for (int xpos = 10; xpos < 1800; xpos += 230) {
    for (int ypos = 10; ypos < 900; ypos += 105) { // NOLINT
      dst_rect.w = static_cast<float>(state->blueortho->w);
      dst_rect.h = static_cast<float>(state->blueortho->h);
      dst_rect.x = static_cast<float>(xpos);
      dst_rect.y = static_cast<float>(ypos);
      SDL_RenderTexture(state->renderer, state->tblueortho, nullptr, &dst_rect);
      dst_rect.w = static_cast<float>(state->blueortho->w) / 2;
      dst_rect.h = static_cast<float>(state->blueortho->h) / 2;
      SDL_RenderTexture(state->renderer, state->tcylinderpurp, nullptr,
                        &dst_rect);
    }
  }

  SDL_RenderPresent(state->renderer);

  return SDL_APP_CONTINUE;
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result) { // NOLINT
  delete static_cast<State *>(appstate);                 // NOLINT
} // NOLINT misc-use-anonymous-namespace
