#ifndef DRAW_H
#define DRAW_H
void draw_title(SDL_Context * context, ADR_Game * game);
void draw_history(SDL_Context * context, ADR_Game * game);
void draw_supplies(SDL_Context * context, ADR_Game * game);
void draw_room(SDL_Context * context, ADR_Game * game);
void draw_border(SDL_Context *context, int x, int y, int width, int height);
#endif
