#include <stdlib.h>
#include <math.h>
#include <SDL.h>
#include <SDL_render.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <chipmunk.h>

#define MALLOC(type, variable_name) type * variable_name = (type *)malloc(sizeof(type))
#define MALLOCA(type) (type *)malloc(sizeof(type))
#define MALLOCS(type, variable_name, count) type * variable_name = (type **)malloc(sizeof(type *) * count)
#define MALLOCSA(type, count) (type **)malloc(sizeof(type *) * count)
#define GAME_WIDTH 1280
#define GAME_HEIGHT 720
#define TIME_PER_TICK 16

typedef struct {
  SDL_Window * window;
  SDL_Renderer * renderer;
  SDL_Texture * resizeTarget;
  SDL_Surface * surface;
  SDL_Event * event;
  SDL_Joystick * joystick;
} SDL_Context;

typedef enum {
  BS_NONE,
  BS_PRESS,
  BS_HOLD,
  BS_RELEASE
} buttonstate;

typedef enum {
  B_EXIT,
  NUMBEROFBUTTONS
} buttons;

typedef struct {
  buttonstate buttons[NUMBEROFBUTTONS];
} HW_Game;

int game_new(HW_Game * game) {
  for (int i = 0; i < NUMBEROFBUTTONS; i++) { game->buttons[i] = BS_NONE; }

  return 0;
}

SDL_Context * sdl_context_new()
{
  MALLOC(SDL_Context, o);

  o->window = SDL_CreateWindow("Hello SDL",
			       SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			       GAME_WIDTH, GAME_HEIGHT,
			       SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

  o->renderer = SDL_CreateRenderer(o->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  o->resizeTarget = SDL_CreateTexture(o->renderer,
				      SDL_PIXELFORMAT_RGBA8888,
				      SDL_TEXTUREACCESS_TARGET,
				      GAME_WIDTH, GAME_HEIGHT);
  o->surface = MALLOCA(SDL_Surface);
  o->event = MALLOCA(SDL_Event);

  if (SDL_NumJoysticks() > 0) { o->joystick = SDL_JoystickOpen(0); }

  return o;
}

void inputs_process_keyboad(SDL_Context *context, HW_Game *game)
{
  int type = context->event->type;
  if (type == SDL_KEYDOWN && context->event->key.repeat == 0) {
  } else if (type == SDL_KEYUP && context->event->key.repeat == 0) {
  } else if (type == SDL_QUIT) {
    game->buttons[B_EXIT] = BS_PRESS;
  }
}

void inputs_process(SDL_Context *context, HW_Game *game)
{
  for (int i = 0; i < NUMBEROFBUTTONS; i++)
  {
    if (game->buttons[i] == BS_PRESS)
      game->buttons[i] = BS_HOLD;
    if (game->buttons[i] == BS_RELEASE)
      game->buttons[i] = BS_NONE;
  }

  while (SDL_PollEvent(context->event)) {
    inputs_process_keyboad(context, game);
  }
}

int main(int argc, char *argv[])
{
  MALLOC(HW_Game, game);
  game_new(game);
  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_AUDIO);
  SDL_Context * context = sdl_context_new();
  SDL_RenderSetScale(context->renderer, 1, 1);
  SDL_SetRenderDrawColor(context->renderer, 0, 0, 0, 255);
  unsigned int accumulator = 0;
  unsigned int last = SDL_GetTicks();

  bool end_game = false;

  while (!game->buttons[B_EXIT]) {
    unsigned int ticks = SDL_GetTicks();
    accumulator += ticks - last;
    last = ticks;
    if (accumulator > TIME_PER_TICK) {
      accumulator -= TIME_PER_TICK;
      inputs_process(context, game);
      SDL_SetRenderTarget(context->renderer, context->resizeTarget);
      SDL_RenderClear(context->renderer);
      SDL_SetRenderTarget(context->renderer, NULL);
      SDL_RenderClear(context->renderer);
      SDL_Rect destrect;
      SDL_RenderGetViewport(context->renderer, &destrect);
      destrect.x = 0;
      destrect.y = 0;
      SDL_RenderCopyEx(context->renderer, context->resizeTarget, NULL, &destrect, 0, NULL, SDL_FLIP_NONE);
      SDL_RenderPresent(context->renderer);
    }
  }

  return 0;
}
