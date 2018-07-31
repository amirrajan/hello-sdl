#include <mruby.h>
#include <mruby/variable.h>
#include <mruby/array.h>
#include <mruby/string.h>
#include <mruby/irep.h>
#include "malloc_macros.h"
#include "mrb_macros.h"
#include "adr_game.h"

int game_new(ADR_Game * game)
{
  for (int i = 0; i < NUMBEROFBUTTONS; i++) { game->buttons[i] = ADR_NONE; }


  return 0;
}
