#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL_image.h>

#include "structs.h"
#include "graphics.h"

#define LEN(arr) ((int) (sizeof (arr) / sizeof (arr)[0]))

void init_LEDs(State *state)
{
    // Initialize 512 "LEDs"
    float y = 0;

    // An LED is light gray by default
    int light_gray[16] = {210, 210, 255, 255};

    // Fill a 2D array with LEDs (data type)
    for (int row = 0; row < 32; ++row)
    {
        float x = 0;
        for (int column = 0; column < 16; ++column)
        {
            state->LEDs[row][column].x = x;
            state->LEDs[row][column].y = y;
            state->LEDs[row][column].w = state->square.w;
            state->LEDs[row][column].h = state->square.h;

            // Color
            state->LEDs[row][column].r = light_gray[0];
            state->LEDs[row][column].g = light_gray[1];
            state->LEDs[row][column].b = light_gray[2];
            state->LEDs[row][column].a = light_gray[3];

            x += state->LEDs[0]->w;
        }
        y += state->LEDs[0]->h;
    }

    // Fill a transposed 2D array with LEDs, in case the user wants to turn
    // Brutus 90 degrees
    transpose(state);


}

void init_animation(State *state)
{
    /***
    Create seperate animation objects for the LED screen and the GUI
    ***/
    state->animation.playing = 0;
    state->animation.current_frame = 0;
    state->animation.current_animation = no_animation;
    state->animation.frame_rate = 8.333333; // ~120 FPS

    // GUI Animation
    state->GUI_animation.playing = 0;
    state->GUI_animation.current_frame = 0;
    state->GUI_animation.current_animation = no_animation;
    state->GUI_animation.frame_rate = 8.333333;
    
    // User animation
    state->animation.user_animation.animation_mode = 0;
    state->animation.user_animation.active_frame = 0;
    state->animation.user_animation.active_color[0] = 255;
    state->animation.user_animation.active_color[1] = 0;
    state->animation.user_animation.active_color[2] = 0;
    state->animation.user_animation.active_color[3] = 255;

}

void init_square(State *state)
{
    state->square.x = 0;
    state->square.y = 0;
    state->square.w = state->settings.LED_width;
    state->square.h = state->settings.LED_height;
}

void init_GUI(State *state, SDL_Renderer *renderer)
{ 
    int standard_button_width = 120;
    int standard_button_height = 32;
    /**************
    Main GUI
    **************/
    state->GUI.x = 0 + state->settings.LED_GUI_width;
    state->GUI.y = 0;
    state->GUI.w = state->settings.display_width - state->settings.LED_GUI_width;
    state->GUI.h = state->settings.display_height;

    // Stop animation button
    state->GUI.button_stop.w = 32;
    state->GUI.button_stop.h = 32;
    state->GUI.button_stop.x = state->settings.display_width - state->GUI.button_stop.w;
    state->GUI.button_stop.y = 0;

    // Animation 1 button
    state->GUI.button_animation_1.x = state->GUI.x + 40;
    state->GUI.button_animation_1.y = state->GUI.y + 40;
    state->GUI.button_animation_1.w = standard_button_width;
    state->GUI.button_animation_1.h = standard_button_height;

    // Randomize button
    state->GUI.button_animation_2.x = state->GUI.x + 40;
    state->GUI.button_animation_2.y = (state->GUI.y + 40) * 2;
    state->GUI.button_animation_2.w = 120;
    state->GUI.button_animation_2.h = 32;

    // Button image
    SDL_Surface *image_surface = NULL;
    image_surface = IMG_Load("button_randomize.jpg");
    if(image_surface == NULL)
    {
        printf("Cannot find image\n");
        SDL_Quit();
    }
    state->GUI.button_animation_1.image_texture = SDL_CreateTextureFromSurface(renderer, image_surface);
    SDL_FreeSurface(image_surface);

    /**************
    User animations GUI element
    **************/

    int margin = 8;
    int margin_small = 4;

    // GUI element rect
    state->GUI.GUI_elem_user_animation.w = state->GUI.w;
    state->GUI.GUI_elem_user_animation.h = state->GUI.h / 4;
    state->GUI.GUI_elem_user_animation.x = state->GUI.x;
    state->GUI.GUI_elem_user_animation.y = state->GUI.h - state->GUI.GUI_elem_user_animation.h;

    // Buttons

    // Test dropdown menu
    int dropdown_h = 32;
    int dropdown_w = 132;
    int dropdown_y = 180;

    state->GUI.test_dropdown.x = 600;
    state->GUI.test_dropdown.y = dropdown_y;
    state->GUI.test_dropdown.w = dropdown_w;
    state->GUI.test_dropdown.h = dropdown_h;
    for (int i = 0; i < LEN(state->GUI.test_dropdown.menu_items); ++i)
    {
        state->GUI.test_dropdown.menu_items[i].x = 600;
        state->GUI.test_dropdown.menu_items[i].y = dropdown_y + dropdown_h * i;
        state->GUI.test_dropdown.menu_items[i].w = dropdown_w;
        state->GUI.test_dropdown.menu_items[i].h = dropdown_h;
    }
    for (int i = 0; i < LEN(state->GUI.test_dropdown.menu_items); ++i)
    {
        printf("menu item %d:   w: %d   h: %d   y: %d\n", i,
                                                          state->GUI.test_dropdown.menu_items[i].w,
                                                          state->GUI.test_dropdown.menu_items[i].h,
                                                          state->GUI.test_dropdown.menu_items[i].y);
    }


    state->GUI.test_dropdown.anim_frame = 0;
    state->GUI.test_dropdown.anim_length = 32;
    state->GUI.test_dropdown.anim_speed = (state->GUI.test_dropdown.menu_items[0].h 
                                           * LEN(state->GUI.test_dropdown.menu_items)
                                           / state->GUI.test_dropdown.anim_length);

    // New animation button
    state->GUI.button_new_animation.x = state->GUI.GUI_elem_user_animation.x + margin;
    state->GUI.button_new_animation.y = state->GUI.GUI_elem_user_animation.y + margin;
    state->GUI.button_new_animation.w = standard_button_width;
    state->GUI.button_new_animation.h = standard_button_height;

    // Save animation button
    state->GUI.button_save_animation.x = state->GUI.button_new_animation.x + 
                                         state->GUI.button_new_animation.w +
                                         margin;
    state->GUI.button_save_animation.y = state->GUI.button_new_animation.y;
    state->GUI.button_save_animation.w = standard_button_width;
    state->GUI.button_save_animation.h = standard_button_height;

    // Timeline
    int max_frames = 32; // For now
    int timeline_y = state->GUI.GUI_elem_user_animation.y + 64;
    int timeline_x = state->GUI.x + margin;
    int frame_button_width = 12;
    int frame_button_height = 32;
    for (int i = 0; i < max_frames; ++i)
    {
        state->GUI.button_animation_frame[i].w = frame_button_width;
        state->GUI.button_animation_frame[i].h = frame_button_height;
        state->GUI.button_animation_frame[i].x = timeline_x + i * (frame_button_width + margin_small);
        state->GUI.button_animation_frame[i].y = timeline_y;
    }

    // Play button (faces the wrong way lol)
    state->GUI.button_play.x = timeline_x;
    state->GUI.button_play.y = timeline_y + frame_button_height + margin;
    state->GUI.button_play.w = 32;
    state->GUI.button_play.h = 32;
    SDL_Surface *play_btn_surface = NULL;
    play_btn_surface = IMG_Load("assets/GUI/button_play.png");
    if(play_btn_surface == NULL)
    {
        printf("Cannot find image\n");
        SDL_Quit();
    }
    state->GUI.button_play.image_texture = SDL_CreateTextureFromSurface(renderer, play_btn_surface);
    SDL_FreeSurface(play_btn_surface);
}

void init_settings(State *state)
{
    state->settings.brutus_width = 16;
    state->settings.brutus_height = 32;
    state->settings.display_width = 960;
    state->settings.display_height = 640;
    state->settings.LED_GUI_width =  320;
    state->settings.LED_GUI_height = state->settings.display_height;
    state->settings.LED_width = state->settings.LED_GUI_width / state->settings.brutus_width; // == 10
    state->settings.LED_height = state->settings.LED_GUI_height / state->settings.brutus_height; // == 10
    printf("Led width, height: %d %d\n", state->settings.LED_width, state->settings.LED_height);
}

void init_user_tools(State *state)
{
    state->user_tools.active_tool = tool_none;
}