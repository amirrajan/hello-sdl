// auto-build: fswatch main.c | xargs -n1 -I{} sh ./build.sh
//             fswatch ./.build-completed | xargs -n1 -I{} ./hello-sdl

#include <mruby.h>
#include <mruby/variable.h>
#include <mruby/array.h>
#include <mruby/string.h>
#include <mruby/irep.h>
#include <stdlib.h>
#include <execinfo.h>
#include <signal.h>
#include <unistd.h>
#include <math.h>
#include <SDL.h>
#include <SDL_render.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <chipmunk.h>

#include "main_ruby.c"
#include "malloc_macros.h"
#include "mrb_macros.h"
#include "adr_game.h"
#include "sdl_context.h"
#include "draw.h"

void inputs_process_keyboad(SDL_Context *context, ADR_Game *game)
{
  int type = context->event->type;

  if (type == SDL_KEYDOWN && context->event->key.repeat == 0) {

  } else if (type == SDL_KEYUP && context->event->key.repeat == 0) {

  } else if (type == SDL_QUIT) {
    game->buttons[B_EXIT] = ADR_PRESS;
  }
}

void inputs_process(SDL_Context *context, ADR_Game *game)
{
  for (int i = 0; i < NUMBEROFBUTTONS; i++) {
    if (game->buttons[i] == ADR_PRESS)   { game->buttons[i] = ADR_HOLD; }
    if (game->buttons[i] == ADR_RELEASE) { game->buttons[i] = ADR_NONE; }
  }

  while (SDL_PollEvent(context->event)) { inputs_process_keyboad(context, game); }
}

void handler(int sig)
{
  void *array[10];
  size_t size;
  size = backtrace(array, 10);
  fprintf(stderr, "Error: signal %d:\n", sig);
  backtrace_symbols_fd(array, size, STDERR_FILENO);
  exit(1);
}

int main(int argc, char *argv[])
{
  signal(SIGSEGV, handler);
  MALLOC(ADR_Game, game);
  game_new(game);

  game->mrb = mrb_open();

  mrb_load_irep(game->mrb, main_ruby);

  struct RClass *game_class = mrb_class_get(game->mrb, "Game");
  game->adr =
    mrb_funcall(game->mrb,
		mrb_obj_value(game_class),
		"new",
		0);

  struct RClass *layout_class = mrb_class_get(game->mrb, "Layout");
  game->layout =
    mrb_funcall(game->mrb,
		mrb_obj_value(layout_class),
		"new",
		2,
		mrb_fixnum_value(GAME_WIDTH),
		mrb_fixnum_value(GAME_HEIGHT));

  if (game->mrb->exc) {
    mrb_value obj = mrb_funcall(game->mrb, mrb_obj_value(game->mrb->exc), "inspect", 0);
    fwrite(RSTRING_PTR(obj), RSTRING_LEN(obj), 1, stdout);
    putc('\n', stdout);
    exit(-1);
  }

  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_AUDIO);
  TTF_Init();
  SDL_Context * context = sdl_context_new();

  unsigned int accumulator = 0;
  unsigned int last = SDL_GetTicks();

  while (!game->buttons[B_EXIT]) {
    unsigned int ticks = SDL_GetTicks();
    accumulator += ticks - last;
    last = ticks;
    if (accumulator > TIME_PER_TICK) {
      SDL_SetRenderDrawColor(context->renderer, 0, 0, 0, 255);
      SDL_RenderClear(context->renderer);
      accumulator -= TIME_PER_TICK;
      inputs_process(context, game);
      /* progress_bar_draw(context, GAME_WIDTH / 2, 50); */
      draw_room(context, game);
      SDL_RenderPresent(context->renderer);
    }
  }

  TTF_Quit();
  SDL_Quit();

  return 0;
}
