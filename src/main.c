#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "headers.h"
#include "graphics.h"

/*

Make:

gcc -std=c11 -Wall -o obj/main.o -c src/main.c
gcc -std=c11 -Wall -o obj/graphics.o -c src/graphics.c
gcc -std=c11 -Wall -o LED_Grid obj/main.o obj/graphics.o -lSDL2 -lGL -lGLEW -lm

*/

#define BRUTUS_WIDTH 320
#define BRUTUS_HEIGHT 640
#define DISPLAY_WIDTH 640
#define DISPLAY_HEIGHT 640

#define LED_NO 512

/*************
Initialize
*************/

void load_program(State *state)
{
    init_settings(state);
    init_square(state);
    init_LEDs(state);
    init_animation(state);
    init_GUI(state);
}

int listen_for_events(SDL_Window *window, State *state, float dt)
{
    void click(State *state);

    /* Listen for events. Return done = 1 if user quits program, done = 0 if not. */
    SDL_Event event;
    int done = 0;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            // Window events
            case SDL_WINDOWEVENT_CLOSE:
            {
                if (window)
                {
                    SDL_DestroyWindow(window);
                    window = NULL;
                    done = 1;
                }
            }
            break;

            // Keyboard events
            case SDL_KEYDOWN:
                {
                    switch (event.key.keysym.sym)
                    {
                        case SDLK_ESCAPE:
                            done = 1;
                        break;
                        case SDLK_p:
                            set_rgba_random_all_LEDs(state);
                        break;
                        case SDLK_a:
                            start_animation(state, test_animation);
                        break;
                    }
                }
            break;

            // Mouse events
            case SDL_MOUSEBUTTONDOWN:
                {
                    switch(event.button.button)
                    {
                        // Left mouse button
                        case SDL_BUTTON_LEFT:
                            click(state);
                        break;
                    }
                }
            break;

            // Quit
            case SDL_QUIT:
                done = 1;
            break;
        }
    }
    return done;
}

void process(State *state, float dt)
{
    // get mouse position
    SDL_GetMouseState(&state->mouse.x, &state->mouse.y);

    do_current_animation(state, test_animation);
}

void render(SDL_Renderer *renderer, State *state)
{
    // Colors
    int blue[16] = {0, 80, 180, 255};
    int light_gray[16] = {210, 210, 255, 255};

    // Draw BG
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Draw 512 LEDs
    for (int row = 0; row < 32; ++row)
    {
        for (int column = 0; column < 16; ++column)
        {
            SDL_SetRenderDrawColor(renderer, state->LEDs[row][column].r,
                                             state->LEDs[row][column].g,
                                             state->LEDs[row][column].b,
                                             state->LEDs[row][column].a);
            SDL_Rect dest = {state->LEDs[row][column].x,
                             state->LEDs[row][column].y,
                             state->LEDs[row][column].w,
                             state->LEDs[row][column].h};
            SDL_RenderFillRect(renderer, &dest);
        }
    }

    // Draw GUI
    SDL_SetRenderDrawColor(renderer, blue[0], blue[1], blue[2], blue[3]);
    SDL_Rect GUI_rect = {state->GUI.x,
                         state->GUI.y,
                         state->GUI.w,
                         state->GUI.h};
    SDL_RenderFillRect(renderer, &GUI_rect);

    /* Buttons */


    // Stop button
    SDL_SetRenderDrawColor(renderer, 150, 0, 0, 255);
    SDL_Rect stop_button_rect = {state->GUI.stop_button.x,
                                 state->GUI.stop_button.y,
                                 state->GUI.stop_button.w,
                                 state->GUI.stop_button.h};
    SDL_RenderFillRect(renderer, &stop_button_rect);

    // Animation 1 button
    SDL_SetRenderDrawColor(renderer, light_gray[0], light_gray[1], light_gray[2], light_gray[3]);
    SDL_Rect anim_1_button_rect = {state->GUI.animation_1_button.x,
                                   state->GUI.animation_1_button.y,
                                   state->GUI.animation_1_button.w,
                                   state->GUI.animation_1_button.h};
    SDL_RenderFillRect(renderer, &anim_1_button_rect);

    // Randomize button
    SDL_Rect rand_button_rect = {state->GUI.randomize_button.x,
                                 state->GUI.randomize_button.y,
                                 state->GUI.randomize_button.w,
                                 state->GUI.randomize_button.h};
    SDL_RenderFillRect(renderer, &rand_button_rect);

    // SDL_RenderCopy(state->renderer, state->GUI.animation_1_button.texture, NULL, &rand_button_rect);

    // Present everything
    SDL_RenderPresent(renderer);
}

/*************
Control
*************/

void click(State *state)
{
    void do_test_animation(State *state);
    int mouse_hovering(State *state, int bx, int by, int bw, int bh);
    void start_animation(State *state, int animation_enum);
    void stop_animation(State *state);
    void set_rgba_random_all_LEDs(State *state);

    // Reset animation before starting a new one
    stop_animation(state);
    set_default_all_LEDs(state);

    // Stop animation button
    if (mouse_hovering(state, state->GUI.stop_button.x,
                              state->GUI.stop_button.y,
                              state->GUI.stop_button.w,
                              state->GUI.stop_button.h))
    {
        stop_animation(state);
        set_default_all_LEDs(state);
    }

    // "Animation 1"-button
    if (mouse_hovering(state, state->GUI.animation_1_button.x,
                              state->GUI.animation_1_button.y,
                              state->GUI.animation_1_button.w,
                              state->GUI.animation_1_button.h))
    {
        start_animation(state, gradient_anim);
    }

    // Randomize button
    if (mouse_hovering(state, state->GUI.randomize_button.x,
                              state->GUI.randomize_button.y,
                              state->GUI.randomize_button.w,
                              state->GUI.randomize_button.h))
    {
        start_animation(state, gradient_anim);
    }

}

int mouse_hovering(State *state, int bx, int by, int bw, int bh)
{
    int ax = state->mouse.x;
    int ay = state->mouse.y;

    if (ax >= bx &&
        ax <= bx + bw &&
        ay >= by &&
        ay <= by + bh)
    {
        return 1;
    }
    return 0;
}

/*************
Main
*************/

int main(int argc, char* argv[])
{
    // Declarations
    void load_program(State *state);
    int listen_for_events(SDL_Window *window, State *state, float dt);
    void process(State *state, float dt);
    void render(SDL_Renderer *renderer, State *state);

    // Declare a state. This is used for interacting with EVERYTHING.
    State state;

    // Initialize
    load_program(&state);

    // Declare SDL objects
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    // Initialize SDL
    SDL_Init(SDL_INIT_VIDEO);

    // Create window.
    // I made the window bigger than 16x32, but kept that aspect ratio.
    window = SDL_CreateWindow("LEDs",                        // Window title
                              SDL_WINDOWPOS_UNDEFINED,              // Initial x position
                              SDL_WINDOWPOS_UNDEFINED,              // Initial y position
                              state.settings.display_width,
                              state.settings.display_height,
                              0);                                   // Flags

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    state.renderer = renderer;

    // Event loop
    int done = 0;

    // Random seed
    srand(time(0));

    // Keep time in sync
    Uint32 time_1 = SDL_GetTicks();
    while (!done)
    {
        // Calculate delta time.
        Uint32 time_2 = SDL_GetTicks();
        float dt = (time_2 - time_1) / 1000.0f;

        // Listen for input.
        if (listen_for_events(window, &state, dt) == 1)
        {
            done = 1;
        }

        // Process
        process(&state, dt);

        // Render stuff.
        render(renderer, &state);

        // Do everything according to delta time.
        time_1 = time_2;
        SDL_Delay(floor(state.animation.frame_rate - dt));
    }

    // Clean up
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    return 0;
}
