#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL_image.h>

#include "structs.h"
#include "graphics.h"


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

            x += state->square.w;
        }
        y += state->square.h;
    }

    // Fill a transposed 2D array with LEDs, in case the user wants to turn
    // Brutus 90 degrees
    transpose(state);


}

void init_animation(State *state)
{
    // Initialize animation helper
    state->animation.playing = 0;
    state->animation.current_frame = 0;
    state->animation.current_animation = no_animation;
    state->animation.frame_rate = 8.333333;
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
    // GUI rect
    state->GUI.x = 0 + state->settings.brutus_width * state->settings.LED_width;
    state->GUI.y = 0;
    state->GUI.w = state->settings.LED_width * state->settings.brutus_width;
    state->GUI.h = state->settings.LED_height * state->settings.brutus_height;

    // Stop animation button rect
    state->GUI.stop_button.w = 32;
    state->GUI.stop_button.h = 32;
    state->GUI.stop_button.x = state->settings.display_width - state->GUI.stop_button.w;
    state->GUI.stop_button.y = 0;

    // Animation 1 button rect
    state->GUI.animation_1_button.x = state->GUI.x + 40;
    state->GUI.animation_1_button.y = state->GUI.y + 40;
    state->GUI.animation_1_button.w = 120;
    state->GUI.animation_1_button.h = 32;

    // Randomize button
    state->GUI.randomize_button.x = state->GUI.x + 40;
    state->GUI.randomize_button.y = (state->GUI.y + 40) * 2;
    state->GUI.randomize_button.w = 120;
    state->GUI.randomize_button.h = 32;

    // Button image
    SDL_Surface *image_surface = NULL;
    image_surface = IMG_Load("button_randomize.jpg");
    if(image_surface == NULL)
    {
        printf("Cannot find image\n");
        SDL_Quit();
    }
    printf("Creating texture from surface\n");
    // for this next function, it will not work to pass state->renderer, you need to pass renderer
    // (thus the need for it to be a parameter in init_GUI())
    state->GUI.animation_1_button.image_texture = SDL_CreateTextureFromSurface(renderer, image_surface);
    SDL_FreeSurface(image_surface);
}

void init_settings(State *state)
{
    state->settings.brutus_width = 16;
    state->settings.brutus_height = 32;
    state->settings.display_width = 640;
    state->settings.display_height = 640;
    state->settings.LED_width = (state->settings.display_width / 2) / state->settings.brutus_width;
    state->settings.LED_height = state->settings.display_height / state->settings.brutus_height;
}
