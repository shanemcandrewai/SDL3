#define SDL_MAIN_USE_CALLBACKS 1 // NOLINT
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

const int static WIDTH = 600;
const int static HEIGHT = 600;
const int static MAX = 255;
const float static SCALE = 4.0F;
const char static *MESSAGE = "Hello, World!"; // NOLINT

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) { // NOLINT

  SDL_Window *window = nullptr;
  SDL_Renderer *renderer = nullptr;
  void *state[2];
  
  /* Create the window */
  if (!SDL_CreateWindowAndRenderer(MESSAGE, WIDTH, HEIGHT, SDL_WINDOW_RESIZABLE,
                                   &window, &renderer)) {
    SDL_Log("Couldn't create window and renderer: %s", // NOLINT
            SDL_GetError());                           // NOLINT
    return SDL_APP_FAILURE;
  }
  state[0] = renderer;
  state[1] = window;
  appstate = (void **)&state;
  return SDL_APP_CONTINUE;
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate) { // NOLINT
// From the debugger I see appstate is empty, it should be pointer to the array passed in at the end of SDL_AppInit
  auto **state = static_cast<void **>(appstate);
  auto *local_renderer = static_cast<SDL_Renderer *>(state[0]);
  int width = 0;
  int height = 0;

  /* Center the message and scale it up */
  SDL_GetRenderOutputSize(local_renderer, &width, &height);
  SDL_SetRenderScale(local_renderer, SCALE, SCALE);
  const float xpos = ((static_cast<float>(width) / SCALE) -
                      (static_cast<float>(SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE) *
                       SDL_strlen(MESSAGE))) /
                     2;
  const float ypos = ((static_cast<float>(height) / SCALE) -
                      SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE) /
                     2;

  /* Draw the message */
  SDL_SetRenderDrawColor(local_renderer, 0, 0, 0, MAX);
  SDL_RenderClear(local_renderer);
  SDL_SetRenderDrawColor(local_renderer, MAX, MAX, MAX, MAX);
  SDL_RenderDebugText(local_renderer, xpos, ypos, MESSAGE);
  SDL_RenderPresent(local_renderer);

  return SDL_APP_CONTINUE;
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) { // NOLINT
  if (event->type == SDL_EVENT_KEY_DOWN || event->type == SDL_EVENT_QUIT) {
    return SDL_APP_SUCCESS; /* end the program, reporting success to the OS. */
  }
  return SDL_APP_CONTINUE;
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result) {
} // NOLINT misc-use-anonymous-namespace
