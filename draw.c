#include <SDL.h>
#include <SDL_render.h>
#include <SDL_image.h>
#include "malloc_macros.h"
#include "mrb_macros.h"
#include "adr_game.h"
#include "sdl_context.h"
#include "draw.h"

void draw_label(SDL_Context *context, char *text, long long x, long long y)
{
  SDL_Color white = { 255, 255, 255 };
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

  SDL_Rect destination_rect = { x - (texture_width / 2),
				y - texture_height,
				texture_width,
				texture_height };

  SDL_RenderCopy(context->renderer,
		 context->texture,
		 NULL,
		 &destination_rect);

  SDL_FreeSurface(context->surface);

  SDL_DestroyTexture(context->texture);
}


void draw_label_sm(SDL_Context *context, char *text, long long x, long long y)
{
  SDL_Color white = { 255, 255, 255 };
  int texture_width = 0;
  int texture_height = 0;

  context->surface = TTF_RenderText_Solid(context->font_sm,
					  text,
					  white);

  context->texture = SDL_CreateTextureFromSurface(context->renderer,
						  context->surface);

  SDL_QueryTexture(context->texture,
		   NULL,
		   NULL,
		   &texture_width,
		   &texture_height);

  SDL_Rect destination_rect = { x,
				y,
				texture_width,
				texture_height };

  SDL_RenderCopy(context->renderer,
		 context->texture,
		 NULL,
		 &destination_rect);

  SDL_FreeSurface(context->surface);

  SDL_DestroyTexture(context->texture);
}

void draw_title(SDL_Context * context, ADR_Game * game)
{
  long long h_x, h_y, h_w, h_h = 0;
  SEND(point, game->layout, title_location, game->mrb);
  long long t_x = mrb_fixnum(mrb_ary_entry(point, 0));
  long long t_y = mrb_fixnum(mrb_ary_entry(point, 1));
  draw_label(context, "a dark room", t_x, t_y);
}

void draw_history(SDL_Context * context, ADR_Game * game)
{
  long long h_x, h_y, h_w, h_h = 0;
  SEND(history_border, game->layout, column_1_frame, game->mrb);
  h_x = mrb_fixnum(mrb_ary_entry(history_border, 0));
  h_y = mrb_fixnum(mrb_ary_entry(history_border, 1));
  h_w = mrb_fixnum(mrb_ary_entry(history_border, 2));
  h_h = mrb_fixnum(mrb_ary_entry(history_border, 3));
  draw_border(context, h_x, h_y, h_w, h_h);

  draw_label_sm(context, "awake. head throbbing.", 60, 60);
  draw_label_sm(context, "vision blurry. the", 60, 75);
  draw_label_sm(context, "voices say to survive.", 60, 90);

  draw_label_sm(context, "the fire is dead.", 60, 130);
  draw_label_sm(context, "the room is cold.", 60, 170);
}

void draw_supplies(SDL_Context * context, ADR_Game * game)
{
  long long h_x, h_y, h_w, h_h = 0;
  SEND(supply_border, game->layout, column_3_frame, game->mrb);
  h_x = mrb_fixnum(mrb_ary_entry(supply_border, 0));
  h_y = mrb_fixnum(mrb_ary_entry(supply_border, 1));
  h_w = mrb_fixnum(mrb_ary_entry(supply_border, 2));
  h_h = mrb_fixnum(mrb_ary_entry(supply_border, 3));
  draw_border(context, h_x, h_y, h_w, h_h);

  draw_label_sm(context, "rusty knife:         1", 980, 60);
}

void draw_room(SDL_Context * context, ADR_Game * game)
{
  /* SEND(room_border, game->layout, column_2_frame, game->mrb); */
  /* h_x = mrb_fixnum(mrb_ary_entry(room_border, 0)); */
  /* h_y = mrb_fixnum(mrb_ary_entry(room_border, 1)); */
  /* h_w = mrb_fixnum(mrb_ary_entry(room_border, 2)); */
  /* h_h = mrb_fixnum(mrb_ary_entry(room_border, 3)); */
  /* border(context, h_x, h_y, h_w, h_h); */

  draw_title(context, game);
  draw_history(context, game);
  draw_supplies(context, game);
}

void draw_border(SDL_Context *context, int x, int y, int width, int height)
{
  SDL_Rect b = { x, y, width, height };
  SDL_SetRenderDrawColor(context->renderer, 255, 255, 255, 255);
  SDL_RenderDrawRect(context->renderer, &b);

  SDL_Rect b2 = { x + 1, y + 1, width - 2, height - 2 };
  SDL_SetRenderDrawColor(context->renderer, 255, 255, 255, 255);
  SDL_RenderDrawRect(context->renderer, &b2);

  /* SDL_Rect b3 = { x + 2, y + 2, width - 4, height - 4 }; */
  /* SDL_SetRenderDrawColor(context->renderer, 255, 255, 255, 255); */
  /* SDL_RenderDrawRect(context->renderer, &b3); */
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

  SDL_Rect destination_rect = { x - (texture_width) / 2,
				y,
				texture_width,
				texture_height };

  SDL_RenderCopy(context->renderer,
		 context->texture,
		 NULL,
		 &destination_rect);

  SDL_FreeSurface(context->surface);

  SDL_DestroyTexture(context->texture);
}
