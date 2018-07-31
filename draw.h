#ifndef DRAW_H
#define DRAW_H
void title_draw(SDL_Context * context, ADR_Game * game);
void history_draw(SDL_Context * context, ADR_Game * game);
void supplies_draw(SDL_Context * context, ADR_Game * game);
void room_draw(SDL_Context * context, ADR_Game * game);
void border(SDL_Context *context, int x, int y, int width, int height);
#endif
