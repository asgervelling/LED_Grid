#ifndef GRAPHICS
#define GRAPHICS

#include "structs.h"

void start_animation(State *state, int animation_enum);
void stop_animation(State *state, int clear_screen);
void set_rgba(int row, int col, int r, int g, int b, int a, State *state);
void set_rgba_random(int row, int col, State *state);
void set_rgba_random_all_LEDs(State* state);
void set_default_all_LEDs(State *state);
void do_current_animation(State *state, int animation); // Called every frame
void ax_plus_b_animation(State *state, int a, int b);
void one_by_one_animation(State *state);
void transpose(State *state);
void print_matrix(State *state);
void gradient_animation(State *state);

// User animation
void store_single_LED(State *state, int LED_row, int LED_column, int frame);
void init_new_animation(State *state, int frames);
void show_animation_frame(State *state);
void store_animation_frame(State *state);


#endif