#include "malloc_macros.h"
#include "adr_game.h"
#include "sdl_context.h"
#include <SDL.h>
#include <SDL_render.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#define GAME_WIDTH 1280 // 2 * 2 * 2 * 2 * 5 * 2 * 2 * 2 * 2
#define GAME_HEIGHT 720 // 2 * 2 * 2 * 2 * 5 * 3 * 3

SDL_Context * sdl_context_new()
{
  MALLOC(SDL_Context, o);

  o->window = SDL_CreateWindow("Hello SDL",
			       SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			       1280, 740,
			       SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

  SDL_SetWindowPosition(o->window, 1750, 750);
  SDL_SetWindowSize(o->window, 1280, 720);

  o->renderer = SDL_CreateRenderer(o->window, -1,
				   SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  o->resizeTarget = SDL_CreateTexture(o->renderer,
				      SDL_PIXELFORMAT_RGBA8888,
				      SDL_TEXTUREACCESS_TARGET,
				      GAME_WIDTH, GAME_HEIGHT);

  o->surface = MALLOCA(SDL_Surface);

  o->event = MALLOCA(SDL_Event);

  o->font = TTF_OpenFont("font.ttf", 18);

  o->font_sm = TTF_OpenFont("font.ttf", 11);

  if (SDL_NumJoysticks() > 0) { o->joystick = SDL_JoystickOpen(0); }

  SDL_RenderSetScale(o->renderer, SCALE, SCALE);

  return o;
}
