#define SDL_MAIN_USE_CALLBACKS 1 /* use the callbacks instead of main() */ //NOLINT
#include <SDL3/SDL.h>  //uncomment for release
#include <SDL3/SDL_main.h>   //keep uncommented for release

const int static WIDTH = 300;
const int static HEIGHT = 300;
const int static MAX = 255;
const float static SCALE = 4.0F;
const char static *MESSAGE = "Hello, wasm 2!"; //NOLINT
const float XPOS = 10;
const float YPOS = 10;

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) { //NOLINT

  SDL_Window *window = nullptr;
  SDL_Renderer *renderer = nullptr;

  if (!SDL_CreateWindowAndRenderer("Hello, wasm!", WIDTH, HEIGHT,
                                   SDL_WINDOW_FULLSCREEN, &window, &renderer)) {
    SDL_Log("Couldn't create window and renderer: %s", SDL_GetError()); //NOLINT
    return SDL_APP_FAILURE;
  }

  if (!SDL_SetRenderVSync(renderer, 1)) {
    SDL_Log("Could not enable VSync! SDL error: %s\n", // NOLINT
            SDL_GetError());
  }
  *appstate = renderer;
  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) { //NOLINT
  if (event->type == SDL_EVENT_KEY_DOWN || event->type == SDL_EVENT_QUIT) {
    return SDL_APP_SUCCESS; /* end the program, reporting success to the OS. */
  }
  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) { //NOLINT
  auto *renderer = static_cast<SDL_Renderer *>(appstate);
  int width = 0;
  int height = 0;

  /* Center the message and scale it up */
  SDL_GetRenderOutputSize(renderer, &width, &height);
  SDL_SetRenderScale(renderer, SCALE, SCALE);

  /* Draw the message */
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, MAX);
  SDL_RenderClear(renderer);
  SDL_SetRenderDrawColor(renderer, MAX, MAX, MAX, MAX);
  SDL_RenderDebugText(renderer, XPOS, YPOS, MESSAGE);
  SDL_RenderPresent(renderer);

  return SDL_APP_CONTINUE;
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result) {}//NOLINT misc-use-anonymous-namespace
