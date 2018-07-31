#ifndef ADR_GAME_H
#define ADR_GAME_H

#define GAME_WIDTH 1280 // 2 * 2 * 2 * 2 * 5 * 2 * 2 * 2 * 2
#define GAME_HEIGHT 720 // 2 * 2 * 2 * 2 * 5 * 3 * 3
#define SCALE 1
#define TIME_PER_TICK 16

#include <mruby.h>
#include <mruby/variable.h>
#include <mruby/array.h>
#include <mruby/string.h>
#include <mruby/irep.h>

typedef enum {
  ADR_NONE,
  ADR_PRESS,
  ADR_HOLD,
  ADR_RELEASE
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
} ADR_Game;

int game_new(ADR_Game * game);
#endif
