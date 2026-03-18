/*
  Copyright (C) 1997-2026 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely.
*/
#define SDL_MAIN_USE_CALLBACKS 1 /* use the callbacks instead of main() */
// #include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_video.h>

const int static WIDTH = 800;
const int static HEIGHT = 800;
const int static MAX = 255;

// static SDL_Window *window = NULL;
// static SDL_Renderer *renderer = NULL;

struct AppState
{
    SDL_Window * window;
    SDL_Renderer * renderer;
};

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
  /* Create the window */

  SDL_Log("%d", argc);
  SDL_Log("%s", argv[0]);
  struct AppState * apps = NULL;
  
  if (!SDL_CreateWindowAndRenderer("Hello World", WIDTH, HEIGHT,
                                   SDL_WINDOW_FULLSCREEN, &apps->window, &apps->renderer)) {
    SDL_Log("Couldn't create window and renderer: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }
  appstate = (void **)apps;
  return SDL_APP_CONTINUE;
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
  void * kkk = appstate;
  if (event->type == SDL_EVENT_KEY_DOWN || event->type == SDL_EVENT_QUIT) {
    return SDL_APP_SUCCESS; /* end the program, reporting success to the OS. */
  }
  return SDL_APP_CONTINUE;
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate) {
  SDL_Log("%s", "appstate");
  struct AppState * apps = appstate;
  // SDL_Window * window = apps->window;
  SDL_Renderer * renderer = apps->renderer;
  const char *message = "Hello World!";
  int width = 0;
  int height = 0;
  const float scale = 4.0F;

  /* Center the message and scale it up */
  SDL_GetRenderOutputSize(renderer, &width, &height);
  SDL_SetRenderScale(renderer, scale, scale);
  float xpos =
      (((float)width / scale) -
       ((float)SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE * SDL_strlen(message))) /
      2;
  float ypos =
      (((float)height / scale) - SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE) / 2;

  /* Draw the message */
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, MAX);
  SDL_RenderClear(renderer);
  SDL_SetRenderDrawColor(renderer, MAX, MAX, MAX, MAX);
  SDL_RenderDebugText(renderer, xpos, ypos, message);
  SDL_RenderPresent(renderer);

  return SDL_APP_CONTINUE;
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result) {}
