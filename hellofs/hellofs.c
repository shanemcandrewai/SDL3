/*
 * Copyright 2014 The Emscripten Authors.  All rights reserved.
 * Emscripten is available under two separate licenses, the MIT license and the
 * University of Illinois/NCSA Open Source License.  Both these licenses can be
 * found in the LICENSE file.
 */

#define SDL_MAIN_USE_CALLBACKS 1 /* use the callbacks instead of main() */
#include <SDL3/SDL.h>  //uncomment for release
#include <SDL3/SDL_main.h>   //keep uncommented for release

#include <assert.h>
#include <stdio.h>
#include <emscripten.h>
#include <string.h>
#include <emscripten/html5.h>
#include <math.h>

const int static WIDTH = 800;
const int static HEIGHT = 800;
const int static MAX = 255;
const float static SCALE = 4.0F;
const char static *MESSAGE = "Hello World5!"; //NOLINT

static inline const char *emscripten_event_type_to_string(int eventType) {
  const char *events[] = { "(invalid)", "(none)", "keypress", "keydown", "keyup", "click", "mousedown", "mouseup", "dblclick", "mousemove", "wheel", "resize",
    "scroll", "blur", "focus", "focusin", "focusout", "deviceorientation", "devicemotion", "orientationchange", "fullscreenchange", "pointerlockchange",
    "visibilitychange", "touchstart", "touchend", "touchmove", "touchcancel", "gamepadconnected", "gamepaddisconnected", "beforeunload",
    "batterychargingchange", "batterylevelchange", "webglcontextlost", "webglcontextrestored", "(invalid)" };
  ++eventType;
  if (eventType < 0) eventType = 0;
  if (eventType >= sizeof(events)/sizeof(events[0])) eventType = sizeof(events)/sizeof(events[0])-1;
  return events[eventType];
}

const char *emscripten_result_to_string(EMSCRIPTEN_RESULT result) {
  if (result == EMSCRIPTEN_RESULT_SUCCESS) return "EMSCRIPTEN_RESULT_SUCCESS";
  if (result == EMSCRIPTEN_RESULT_DEFERRED) return "EMSCRIPTEN_RESULT_DEFERRED";
  if (result == EMSCRIPTEN_RESULT_NOT_SUPPORTED) return "EMSCRIPTEN_RESULT_NOT_SUPPORTED";
  if (result == EMSCRIPTEN_RESULT_FAILED_NOT_DEFERRED) return "EMSCRIPTEN_RESULT_FAILED_NOT_DEFERRED";
  if (result == EMSCRIPTEN_RESULT_INVALID_TARGET) return "EMSCRIPTEN_RESULT_INVALID_TARGET";
  if (result == EMSCRIPTEN_RESULT_UNKNOWN_TARGET) return "EMSCRIPTEN_RESULT_UNKNOWN_TARGET";
  if (result == EMSCRIPTEN_RESULT_INVALID_PARAM) return "EMSCRIPTEN_RESULT_INVALID_PARAM";
  if (result == EMSCRIPTEN_RESULT_FAILED) return "EMSCRIPTEN_RESULT_FAILED";
  if (result == EMSCRIPTEN_RESULT_NO_DATA) return "EMSCRIPTEN_RESULT_NO_DATA";
  return "Unknown EMSCRIPTEN_RESULT!";
}

#define TEST_RESULT(x) if (ret != EMSCRIPTEN_RESULT_SUCCESS) { \
    printf("%s returned %s.\n", #x, emscripten_result_to_string(ret)); \
    assert(false && #x); \
  }

// The event handler functions can return 1 to suppress the event and disable the default action. That calls event.preventDefault();
// Returning 0 signals that the event was not consumed by the code, and will allow the event to pass on and bubble up normally.
bool key_callback(int eventType, const EmscriptenKeyboardEvent *e, void *userData) {
  if (eventType == EMSCRIPTEN_EVENT_KEYPRESS && (!strcmp(e->key, "f") || e->which == 102)) {
    EmscriptenFullscreenChangeEvent fsce;
    EMSCRIPTEN_RESULT ret = emscripten_get_fullscreen_status(&fsce);
    TEST_RESULT(emscripten_get_fullscreen_status);
    if (!fsce.isFullscreen) {
      printf("Requesting fullscreen..\n");
      ret = emscripten_request_fullscreen("#canvas", 1);
      TEST_RESULT(emscripten_request_fullscreen);
    } else {
      printf("Exiting fullscreen..\n");
      ret = emscripten_exit_fullscreen();
      TEST_RESULT(emscripten_exit_fullscreen);
      ret = emscripten_get_fullscreen_status(&fsce);
      TEST_RESULT(emscripten_get_fullscreen_status);
      if (fsce.isFullscreen) {
        fprintf(stderr, "Fullscreen exit did not work!\n");
      }
    }
  } else if (eventType == EMSCRIPTEN_EVENT_KEYPRESS && (!strcmp(e->key, "Esc") || !strcmp(e->key, "Escape") || e->which == 27)) {
    emscripten_exit_soft_fullscreen();
  }
  return 0;
}

int callCount = 0;

bool fullscreenchange_callback(int eventType, const EmscriptenFullscreenChangeEvent *e, void *userData) {
  printf("%s, isFullscreen: %d, fullscreenEnabled: %d, fs element nodeName: "
         "'%s', fs element id: '%s'. New size: %dx%d pixels. Screen size: "
         "%dx%d pixels.\n",
         emscripten_event_type_to_string(eventType),
         e->isFullscreen,
         e->fullscreenEnabled,
         e->nodeName,
         e->id,
         e->elementWidth,
         e->elementHeight,
         e->screenWidth,
         e->screenHeight);

  ++callCount;
  if (callCount == 1) { // Transitioned to fullscreen.
    assert(e->isFullscreen);
  } else if (callCount == 2) { // Transitioned to windowed, we must be back to the default pixel size 300x150.
    assert(!e->isFullscreen);
  }
  return 0;
}

bool mouse_callback(int eventType, const EmscriptenMouseEvent *e, void *userData) {
  return 0;
}

bool on_canvassize_changed(int eventType, const void *reserved, void *userData) {
  int w, h;
  emscripten_get_canvas_element_size("#canvas", &w, &h);
  double cssW, cssH;
  emscripten_get_element_css_size("#canvas", &cssW, &cssH);
  printf("Canvas resized: WebGL RTT size: %dx%d, canvas CSS size: %02gx%02g\n", w, h, cssW, cssH);
  return 0;
}

  void requestFullscreen(int scaleMode, int canvasResolutionScaleMode, int filteringMode) {
    EmscriptenFullscreenStrategy s;
    memset(&s, 0, sizeof(s));
    s.scaleMode = scaleMode;
    s.canvasResolutionScaleMode = canvasResolutionScaleMode;
    s.filteringMode = filteringMode;
    s.canvasResizedCallback = on_canvassize_changed;
    EMSCRIPTEN_RESULT ret = emscripten_request_fullscreen_strategy("#canvas", 1, &s);
    TEST_RESULT(requestFullscreen);
  }

void enterSoftFullscreen(int scaleMode, int canvasResolutionScaleMode, int filteringMode) {
  EmscriptenFullscreenStrategy s;
  memset(&s, 0, sizeof(s));
  s.scaleMode = scaleMode;
  s.canvasResolutionScaleMode = canvasResolutionScaleMode;
  s.filteringMode = filteringMode;
  s.canvasResizedCallback = on_canvassize_changed;
  EMSCRIPTEN_RESULT ret = emscripten_enter_soft_fullscreen("#canvas", &s);
  TEST_RESULT(enterSoftFullscreen);
}

bool on_button_click(int eventType, const EmscriptenMouseEvent *mouseEvent, void *userData) {
  printf("on_button_click: %ld\n", (long)userData);
  switch ((long)userData) {
    case 0: requestFullscreen(EMSCRIPTEN_FULLSCREEN_SCALE_DEFAULT, EMSCRIPTEN_FULLSCREEN_CANVAS_SCALE_NONE, EMSCRIPTEN_FULLSCREEN_FILTERING_DEFAULT); break;
    case 1: requestFullscreen(EMSCRIPTEN_FULLSCREEN_SCALE_STRETCH, EMSCRIPTEN_FULLSCREEN_CANVAS_SCALE_STDDEF, EMSCRIPTEN_FULLSCREEN_FILTERING_DEFAULT); break;
    case 2: requestFullscreen(EMSCRIPTEN_FULLSCREEN_SCALE_STRETCH, EMSCRIPTEN_FULLSCREEN_CANVAS_SCALE_HIDEF, EMSCRIPTEN_FULLSCREEN_FILTERING_DEFAULT); break;
    case 3: requestFullscreen(EMSCRIPTEN_FULLSCREEN_SCALE_ASPECT, EMSCRIPTEN_FULLSCREEN_CANVAS_SCALE_STDDEF, EMSCRIPTEN_FULLSCREEN_FILTERING_DEFAULT); break;
    case 4: requestFullscreen(EMSCRIPTEN_FULLSCREEN_SCALE_ASPECT, EMSCRIPTEN_FULLSCREEN_CANVAS_SCALE_HIDEF, EMSCRIPTEN_FULLSCREEN_FILTERING_DEFAULT); break;
    case 5: requestFullscreen(EMSCRIPTEN_FULLSCREEN_SCALE_STRETCH, EMSCRIPTEN_FULLSCREEN_CANVAS_SCALE_NONE, EMSCRIPTEN_FULLSCREEN_FILTERING_DEFAULT); break;
    case 6: requestFullscreen(EMSCRIPTEN_FULLSCREEN_SCALE_ASPECT, EMSCRIPTEN_FULLSCREEN_CANVAS_SCALE_NONE, EMSCRIPTEN_FULLSCREEN_FILTERING_DEFAULT); break;
    case 7: requestFullscreen(EMSCRIPTEN_FULLSCREEN_SCALE_CENTER, EMSCRIPTEN_FULLSCREEN_CANVAS_SCALE_NONE, EMSCRIPTEN_FULLSCREEN_FILTERING_DEFAULT); break;

    case 8: enterSoftFullscreen(EMSCRIPTEN_FULLSCREEN_SCALE_STRETCH, EMSCRIPTEN_FULLSCREEN_CANVAS_SCALE_STDDEF, EMSCRIPTEN_FULLSCREEN_FILTERING_DEFAULT); break;
    case 9: enterSoftFullscreen(EMSCRIPTEN_FULLSCREEN_SCALE_STRETCH, EMSCRIPTEN_FULLSCREEN_CANVAS_SCALE_HIDEF, EMSCRIPTEN_FULLSCREEN_FILTERING_DEFAULT); break;
    case 10: enterSoftFullscreen(EMSCRIPTEN_FULLSCREEN_SCALE_ASPECT, EMSCRIPTEN_FULLSCREEN_CANVAS_SCALE_STDDEF, EMSCRIPTEN_FULLSCREEN_FILTERING_DEFAULT); break;
    case 11: enterSoftFullscreen(EMSCRIPTEN_FULLSCREEN_SCALE_ASPECT, EMSCRIPTEN_FULLSCREEN_CANVAS_SCALE_HIDEF, EMSCRIPTEN_FULLSCREEN_FILTERING_DEFAULT); break;
    case 12: enterSoftFullscreen(EMSCRIPTEN_FULLSCREEN_SCALE_STRETCH, EMSCRIPTEN_FULLSCREEN_CANVAS_SCALE_NONE, EMSCRIPTEN_FULLSCREEN_FILTERING_DEFAULT); break;
    case 13: enterSoftFullscreen(EMSCRIPTEN_FULLSCREEN_SCALE_ASPECT, EMSCRIPTEN_FULLSCREEN_CANVAS_SCALE_NONE, EMSCRIPTEN_FULLSCREEN_FILTERING_DEFAULT); break;
    case 14: enterSoftFullscreen(EMSCRIPTEN_FULLSCREEN_SCALE_CENTER, EMSCRIPTEN_FULLSCREEN_CANVAS_SCALE_NONE, EMSCRIPTEN_FULLSCREEN_FILTERING_DEFAULT); break;

    case 15: requestFullscreen(EMSCRIPTEN_FULLSCREEN_SCALE_ASPECT, EMSCRIPTEN_FULLSCREEN_CANVAS_SCALE_NONE, EMSCRIPTEN_FULLSCREEN_FILTERING_NEAREST); break;
    case 16: enterSoftFullscreen(EMSCRIPTEN_FULLSCREEN_SCALE_ASPECT, EMSCRIPTEN_FULLSCREEN_CANVAS_SCALE_NONE, EMSCRIPTEN_FULLSCREEN_FILTERING_NEAREST); break;
    default: return 0;
  }
  return 1;
}

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) { //NOLINT

  SDL_Window *window = NULL;
  SDL_Renderer *renderer = NULL;

  /* Create the window */
  if (!SDL_CreateWindowAndRenderer("Hello World", WIDTH, HEIGHT,
                                   SDL_WINDOW_FULLSCREEN, &window, &renderer)) {
    SDL_Log("Couldn't create window and renderer: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }
  // Enable VSync
  if (!SDL_SetRenderVSync(renderer, 1)) {
    SDL_Log("Could not enable VSync! SDL error: %s\n", // NOLINT
            SDL_GetError());
  }


  EMSCRIPTEN_RESULT ret = emscripten_set_keypress_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, 1, key_callback);
  TEST_RESULT(emscripten_set_keypress_callback);

  ret = emscripten_set_fullscreenchange_callback(EMSCRIPTEN_EVENT_TARGET_DOCUMENT, 0, 1, fullscreenchange_callback);
  TEST_RESULT(emscripten_set_fullscreenchange_callback);

  // For Internet Explorer, fullscreen and pointer lock requests cannot be run
  // from inside keyboard event handlers. Therefore we must register a callback to
  // mouse events (any other than mousedown) to activate deferred fullscreen/pointerlock
  // requests to occur for IE. The callback itself can be a no-op.
  // ret = emscripten_set_click_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, 1, mouse_callback);
  // TEST_RESULT(emscripten_set_click_callback);
  // ret = emscripten_set_mousedown_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, 1, mouse_callback);
  // TEST_RESULT(emscripten_set_mousedown_callback);
  // ret = emscripten_set_mouseup_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, 1, mouse_callback);
  // TEST_RESULT(emscripten_set_mouseup_callback);
  // ret = emscripten_set_dblclick_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, 1, mouse_callback);
  // TEST_RESULT(emscripten_set_dblclick_callback);

  // emscripten_set_click_callback("#b0", (void*)0, 1, on_button_click);
  // emscripten_set_click_callback("#b1", (void*)1, 1, on_button_click);
  // emscripten_set_click_callback("#b2", (void*)2, 1, on_button_click);
  // emscripten_set_click_callback("#b3", (void*)3, 1, on_button_click);
  // emscripten_set_click_callback("#b4", (void*)4, 1, on_button_click);
  // emscripten_set_click_callback("#b5", (void*)5, 1, on_button_click);
  // emscripten_set_click_callback("#b6", (void*)6, 1, on_button_click);
  // emscripten_set_click_callback("#b7", (void*)7, 1, on_button_click);
  // emscripten_set_click_callback("#b8", (void*)8, 1, on_button_click);
  // emscripten_set_click_callback("#b9", (void*)9, 1, on_button_click);
  // emscripten_set_click_callback("#b10", (void*)10, 1, on_button_click);
  // emscripten_set_click_callback("#b11", (void*)11, 1, on_button_click);
  // emscripten_set_click_callback("#b12", (void*)12, 1, on_button_click);
  // emscripten_set_click_callback("#b13", (void*)13, 1, on_button_click);
  // emscripten_set_click_callback("#b14", (void*)14, 1, on_button_click);
  // emscripten_set_click_callback("#b15", (void*)15, 1, on_button_click);
  // emscripten_set_click_callback("#b16", (void*)16, 1, on_button_click);

  printf("To finish this test, press f to enter fullscreen mode, and then exit it.\n");


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
SDL_AppResult SDL_AppIterate(void *appstate) {
  SDL_Renderer *renderer = appstate;
  int width = 0;
  int height = 0;

  /* Center the message and scale it up */
  SDL_GetRenderOutputSize(renderer, &width, &height);
  SDL_SetRenderScale(renderer, SCALE, SCALE);
  float xpos =
      (((float)width / SCALE) -
       ((float)SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE * SDL_strlen(MESSAGE))) /
      2;
  float ypos =
      (((float)height / SCALE) - SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE) / 2;

  /* Draw the message */
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, MAX);
  SDL_RenderClear(renderer);
  SDL_SetRenderDrawColor(renderer, MAX, MAX, MAX, MAX);
  SDL_RenderDebugText(renderer, xpos, ypos, MESSAGE);
  SDL_RenderPresent(renderer);

  return SDL_APP_CONTINUE;
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result) {}

