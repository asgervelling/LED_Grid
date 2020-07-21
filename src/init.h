#ifndef INIT
#define INIT

#include "structs.h"

void init_settings(State *state);
void init_LEDs(State *state);
void init_animation(State *state);
void init_square(State *state);
void init_GUI(State *state, SDL_Renderer *renderer);
void init_user_tools(State *state);



#endif