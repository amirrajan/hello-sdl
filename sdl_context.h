#ifndef SDL_CONTEXT_H
#define SDL_CONTEXT_H

#include <SDL_ttf.h>

typedef struct {
  SDL_Window * window;
  SDL_Renderer * renderer;
  SDL_Texture * resizeTarget;
  SDL_Texture * texture;
  SDL_Surface * surface;
  SDL_Event * event;
  SDL_Joystick * joystick;
  TTF_Font * font;
  TTF_Font * font_sm;
} SDL_Context;

SDL_Context * sdl_context_new();

#endif
