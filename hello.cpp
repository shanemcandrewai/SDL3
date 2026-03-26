/*
  SDL3 hello.cpp
  adapted from https://github.com/libsdl-org/SDL/blob/main/docs/hello.c
  cls && clang-tidy hello.c -checks=* --header-filter=ddd --
  -ISDL3-3.4.2\include

  Copyright (C) 1997-2026 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely.

*/
#define SDL_MAIN_USE_CALLBACKS 1 /* use the callbacks instead of main() */ //NOLINT
// #include <SDL3/SDL.h>  //uncomment for release
#include <SDL3/SDL_error.h>  //clang-tidy
#include <SDL3/SDL_events.h> //clang-tidy
#include <SDL3/SDL_init.h>   //clang-tidy
#include <SDL3/SDL_log.h>    //clang-tidy
#include <SDL3/SDL_main.h>   //keep uncommented for release
#include <SDL3/SDL_render.h> //clang-tidy
#include <SDL3/SDL_stdinc.h> //clang-tidy
#include <SDL3/SDL_video.h>  //clang-tidy

const int static WIDTH = 800;
const int static HEIGHT = 800;
const int static MAX = 255;
const float static SCALE = 4.0F;
const char *MESSAGE = "Hello World!4"; //NOLINT

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) { //NOLINT

  SDL_Window *window = nullptr;
  SDL_Renderer *renderer = nullptr;

  /* Create the window */
  if (!SDL_CreateWindowAndRenderer("Hello World", WIDTH, HEIGHT,
                                   SDL_WINDOW_FULLSCREEN, &window, &renderer)) {
    SDL_Log("Couldn't create window and renderer: %s", SDL_GetError()); //NOLINT
    return SDL_APP_FAILURE;
  }
  *appstate = renderer;
  return SDL_APP_CONTINUE;
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) { //NOLINT
  if (event->type == SDL_EVENT_KEY_DOWN || event->type == SDL_EVENT_QUIT) {
    return SDL_APP_SUCCESS; /* end the program, reporting success to the OS. */
  }
  return SDL_APP_CONTINUE;
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate) { //NOLINT
  auto *renderer = static_cast<SDL_Renderer *>(appstate);
  int width = 0;
  int height = 0;

  /* Center the message and scale it up */
  SDL_GetRenderOutputSize(renderer, &width, &height);
  SDL_SetRenderScale(renderer, SCALE, SCALE);
  const float xpos = ((static_cast<float>(width) / SCALE) -
                (static_cast<float>(SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE) *
                 SDL_strlen(MESSAGE))) /
               2;
  const float ypos = ((static_cast<float>(height) / SCALE) -
                SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE) /
               2;

  /* Draw the message */
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, MAX);
  SDL_RenderClear(renderer);
  SDL_SetRenderDrawColor(renderer, MAX, MAX, MAX, MAX);
  SDL_RenderDebugText(renderer, xpos, ypos, MESSAGE);
  SDL_RenderPresent(renderer);

  return SDL_APP_CONTINUE;
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result) {}//NOLINT misc-use-anonymous-namespace
