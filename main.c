// auto-build: fswatch main.c | xargs -n1 -I{} sh ./build.sh
//             fswatch ./.build-completed | xargs -n1 -I{} ./hello-sdl

#include <mruby.h>
#include <mruby/variable.h>
#include <mruby/array.h>
#include <mruby/string.h>
#include <mruby/irep.h>
#include "main_ruby.c"
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


#define MALLOC(type, variable_name) type * variable_name = (type *)malloc(sizeof(type))
#define MALLOCA(type) (type *)malloc(sizeof(type))
#define MALLOCS(type, variable_name, count) type * variable_name = (type **)malloc(sizeof(type *) * count)
#define MALLOCSA(type, count) (type **)malloc(sizeof(type *) * count)
#define SCALE 1
#define GAME_WIDTH 1280 / SCALE
#define GAME_HEIGHT 720 / SCALE
#define TIME_PER_TICK 16

typedef struct {
  SDL_Window * window;
  SDL_Renderer * renderer;
  SDL_Texture * resizeTarget;
  SDL_Texture * texture;
  SDL_Surface * surface;
  SDL_Event * event;
  SDL_Joystick * joystick;
  TTF_Font * font;
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
  mrb_state *mrb;
  mrb_value adr;
  mrb_value layout;
} HW_Game;

int game_new(HW_Game * game)
{
  for (int i = 0; i < NUMBEROFBUTTONS; i++) { game->buttons[i] = BS_NONE; }

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
    SDL_Log("not worky");
    putc('\n', stdout);
  }

  return 0;
}

SDL_Context * sdl_context_new()
{
  MALLOC(SDL_Context, o);

  o->window = SDL_CreateWindow("Hello SDL",
			       SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			       GAME_WIDTH, GAME_HEIGHT,
			       SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

  SDL_SetWindowPosition(o->window, 675, 375);

  o->renderer = SDL_CreateRenderer(o->window, -1,
				   SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  o->resizeTarget = SDL_CreateTexture(o->renderer,
				      SDL_PIXELFORMAT_RGBA8888,
				      SDL_TEXTUREACCESS_TARGET,
				      GAME_WIDTH, GAME_HEIGHT);

  o->surface = MALLOCA(SDL_Surface);

  o->event = MALLOCA(SDL_Event);

  o->font = TTF_OpenFont("font.ttf", 25);

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
  for (int i = 0; i < NUMBEROFBUTTONS; i++) {
    if (game->buttons[i] == BS_PRESS)   { game->buttons[i] = BS_HOLD; }
    if (game->buttons[i] == BS_RELEASE) { game->buttons[i] = BS_NONE; }
  }

  while (SDL_PollEvent(context->event)) { inputs_process_keyboad(context, game); }
}

void progress_bar_draw(SDL_Context *context, int x, int y)
{
  int texture_width = 0;
  int texture_height = 0;

  context->surface = IMG_Load("progress-bar.png");

  context->texture = SDL_CreateTextureFromSurface(context->renderer,
						  context->surface);

  SDL_SetColorKey(context->surface,
		  SDL_TRUE,
		  SDL_MapRGB(context->surface->format, 0x00, 0x40, 0x80));

  SDL_QueryTexture(context->texture,
		   NULL,
		   NULL,
		   &texture_width,
		   &texture_height);

  SDL_Rect destination_rect = { x - (texture_width / SCALE) / 2,
				y,
				texture_width / SCALE,
				texture_height / SCALE };

  SDL_RenderCopy(context->renderer,
		 context->texture,
		 NULL,
		 &destination_rect);

  SDL_FreeSurface(context->surface);

  SDL_DestroyTexture(context->texture);
}

void label_draw(SDL_Context *context, char *text, long long x, long long y)
{
  SDL_Color white = { 0, 0, 0 };
  int texture_width = 0;
  int texture_height = 0;

  context->surface = TTF_RenderText_Solid(context->font,
					  text,
					  white);

  context->texture = SDL_CreateTextureFromSurface(context->renderer,
						  context->surface);

  SDL_QueryTexture(context->texture,
		   NULL,
		   NULL,
		   &texture_width,
		   &texture_height);

  SDL_Rect destination_rect = { x - (texture_width / SCALE) / 2,
				y,
				texture_width / SCALE,
				texture_height / SCALE };

  SDL_RenderCopy(context->renderer,
		 context->texture,
		 NULL,
		 &destination_rect);

  SDL_FreeSurface(context->surface);

  SDL_DestroyTexture(context->texture);
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

void room_draw(SDL_Context * context, HW_Game * game)
{
  mrb_value point = mrb_funcall(game->mrb,
				game->layout,
				"title_location",
				0);

  if (game->mrb->exc) {
    mrb_value obj = mrb_funcall(game->mrb, mrb_obj_value(game->mrb->exc), "inspect", 0);
    fwrite(RSTRING_PTR(obj), RSTRING_LEN(obj), 1, stdout);
    SDL_Log("not worky");
    putc('\n', stdout);
  }

  long long x = mrb_fixnum(mrb_ary_entry(point, 0));
  long long y = mrb_fixnum(mrb_ary_entry(point, 1));

  label_draw(context, "a dark room", x, y);
}

int main(int argc, char *argv[])
{
  signal(SIGSEGV, handler);
  MALLOC(HW_Game, game);
  game_new(game);
  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_AUDIO);
  TTF_Init();
  SDL_Context * context = sdl_context_new();
  SDL_RenderSetScale(context->renderer, 1, 1);
  SDL_SetRenderDrawColor(context->renderer, 255, 255, 255, 255);

  unsigned int accumulator = 0;
  unsigned int last = SDL_GetTicks();

  while (!game->buttons[B_EXIT]) {
    unsigned int ticks = SDL_GetTicks();
    accumulator += ticks - last;
    last = ticks;
    if (accumulator > TIME_PER_TICK) {
      SDL_RenderClear(context->renderer);
      accumulator -= TIME_PER_TICK;
      inputs_process(context, game);
      room_draw(context, game);
      /* progress_bar_draw(context, GAME_WIDTH / 2, 50); */
      SDL_RenderPresent(context->renderer);
    }
  }

  TTF_Quit();
  SDL_Quit();

  return 0;
}
