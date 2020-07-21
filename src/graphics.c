#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "structs.h"


#define LEN(arr) ((int) (sizeof (arr) / sizeof (arr)[0]))

/*************
Animation
*************/

#define LED_NO 512

void start_animation(State *state, int animation_enum)
{
    state->animation.playing = 1;
    state->animation.current_animation = animation_enum;
}

void stop_animation(State *state, int clear_screen)
{
    void set_default_all_LEDs(State *state);
    state->animation.playing = 0;
    state->animation.current_animation = no_animation;
    state->animation.animation_frame = -1;

    if (clear_screen == 1)
    {
        set_default_all_LEDs(state);
    }
}

void set_rgba(int row, int col, int r, int g, int b, int a, State *state)
{
    // Set the r, g, b and a of a single LED
    state->LEDs[row][col].r = r;
    state->LEDs[row][col].g = g;
    state->LEDs[row][col].b = b;
    state->LEDs[row][col].a = a;
}

void set_rgba_random(int row, int col, State *state)
{
    // Get random rgba values
    int rand_r = rand() % 256;
    int rand_g = rand() % 256;
    int rand_b = rand() % 256;
    int rand_a = rand() % 256;

    // Set the chosen LED's color to those values
    set_rgba(row, col, rand_r, rand_g, rand_b, rand_a, state);
}

void set_rgba_random_all_LEDs(State* state)
{
    for (int row = 0; row < 32; ++row)
    {
        for (int column = 0; column < 16; ++column)
        {
            set_rgba_random(row, column, state);
        }
    }
}

void gradient_animation(State *state)
{
    int rgba = state->animation.animation_frame;
    if (state->animation.animation_frame > 255)
    {
        rgba = 255 - rgba;
    }
    if (state->animation.animation_frame > 511)
    {
        stop_animation(state, 0);
        return;
    }
    int row = 31 - (floor(state->animation.animation_frame / 16));
    int col = state->animation.animation_frame % 16;

    set_rgba(row, col, rgba, rgba, rgba, 255, state);
}

void do_test_animation(State *state)
{
    void set_default_all_LEDs(State *state);
    if (state->animation.animation_frame >= 32)
    {
        stop_animation(state, 0);
        return;
    }
    int blue[16] = {0, 80, 180, 255};

    set_default_all_LEDs(state);
    set_rgba(state->animation.animation_frame, state->animation.animation_frame, blue[0], blue[1], blue[2], blue[3], state);
    printf("anim frame: %d\n", state->animation.animation_frame);
    
}

void one_by_one_animation(State *state)
{
    if (state->animation.animation_frame >= LED_NO)
    {
        stop_animation(state, 0);
        return;
    }
    int row = 31 - (floor(state->animation.animation_frame / 16));
    int col = state->animation.animation_frame % 16;

    set_rgba_random(row, col, state);
}

void set_default_all_LEDs(State *state)
{
    int light_gray[16] = {210, 210, 255, 255};
    for (int row = 0; row < 32; ++row)
    {
        for (int column = 0; column < 16; ++column)
        {
            set_rgba(row, column, light_gray[0], light_gray[1], light_gray[2], light_gray[3], state);
        }
    }
}

void do_current_animation(State *state, int animation) // Called every frame
{
    void do_test_animation(State *state);
    void ax_plus_b_animation(State *state, int a, int b);
    void gradient_animation(State *state);

    if (state->animation.playing == 0)
    {
        state->animation.animation_frame = 0;
        state->animation.current_frame = 0;
        return;
    }

    // Set the speed of the animation
    int frame_length_ms = 1;
    state->animation.current_animation = animation;
    state->animation.current_frame += 1;

    // Set animation speed according to frame length
    if (state->animation.current_frame % frame_length_ms == 0)
    {
        state->animation.animation_frame += 1;
    }
    // printf("Frame: %d\nAnimation frame: %d\n\n", state->animation.current_frame, state->animation.animation_frame);

    // The animations are currently stored in an enum
    if (state->animation.current_animation == test_animation)
    {
        // do_test_animation(state);
        one_by_one_animation(state);
    }
    if (state->animation.current_animation == ax_plus_b_anim)
    {
        ax_plus_b_animation(state, 2, 4);
    }
    if (state->animation.current_animation == gradient_anim)
    {
        gradient_animation(state);
    }

}

void ax_plus_b_animation(State *state, int a, int b)
{
    set_rgba(a, 2, 0, 255, 0, 255, state);
}

/*******************
 * Math
 * ****************/


int y_flipped(State *state, int y)
{
    // Use this tet set (0, 0) as the bottom left
    y = state->settings.display_height - y;
    return y;
}

void transpose(State *state)
{
    // [[a, b] == [a, c]
    //  [c, d]    [b, d]]
    for (int row = 0; row < 32; ++row)
    {
        for (int column = 0; column < 16; ++column)
        {
            state->LEDs_Transposed[column][row] = state->LEDs[row][column];
        }
    }
    
}

void print_matrix(State *state)
{
    printf("LEDs:\n");
    for (int row = 0; row < 32; ++row)
    {
        for (int column = 0; column < 16; ++column)
        {
            printf("%d ", state->LEDs[row][column].r);
        }
        printf("\n");
    }

    printf("LEDs_Transposed:\n");
    for (int row = 0; row < 16; ++row)
    {
        for (int column = 0; column < 32; ++column)
        {
            printf("%d ", state->LEDs_Transposed[row][column].r);
        }
        printf("\n");
    }
    printf("Number of rows: %d\n", LEN(state->LEDs));
    printf("Number of columns: %d\n", LEN(state->LEDs[0]));

    printf("Number of rows: %d\n", LEN(state->LEDs_Transposed));
    printf("Number of columns: %d\n", LEN(state->LEDs_Transposed[0]));
}

/****************************
User animation
****************************/

void store_single_LED(State *state, int LED_row, int LED_column, int frame)
{
    int color = state->animation.user_animation.active_color;
    // Essentially a 3D array
    state->animation.user_animation.frames[frame].LEDs[LED_row][LED_column].r = state->animation.user_animation.active_color[0];
    state->animation.user_animation.frames[frame].LEDs[LED_row][LED_column].g = state->animation.user_animation.active_color[1];
    state->animation.user_animation.frames[frame].LEDs[LED_row][LED_column].b = state->animation.user_animation.active_color[2];
    state->animation.user_animation.frames[frame].LEDs[LED_row][LED_column].a = state->animation.user_animation.active_color[3];
}

void init_new_animation(State *state, int frames)
{
    // Set all frames in a new animation to the default color
    int light_gray[16] = {210, 210, 255, 255};
    for (int i = 0; i < 3; ++i)
    {
        state->animation.user_animation.active_color[i] = light_gray[i];
    }


    for (int frame = 0; frame < frames; ++frame)
    {
        for (int row = 0; row < 32; ++row)
        {
            for (int column = 0; column < 16; ++column)
            {
                store_single_LED(state,
                                 row,
                                 column,
                                 frame);
            }
        }
    }
    printf("initialized new anim\n");
}

void show_animation_frame(State *state)
{
    for (int row = 0; row < 32; ++row)
    {
        for (int column = 0; column < 16; ++column)
        {
            set_rgba(row,
                     column,
                     state->animation.user_animation.frames[state->animation.user_animation.active_frame].LEDs[row][column].r,
                     state->animation.user_animation.frames[state->animation.user_animation.active_frame].LEDs[row][column].g,
                     state->animation.user_animation.frames[state->animation.user_animation.active_frame].LEDs[row][column].b,
                     state->animation.user_animation.frames[state->animation.user_animation.active_frame].LEDs[row][column].a,
                     state);
        }
    }
}

void store_animation_frame(State *state)
{
    printf("Active frame: %d\n", state->animation.user_animation.active_frame);
}