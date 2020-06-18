#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "sdl.h"
#include "headers/structs.h"
#include "headers/animation.h"

#define DISPLAY_WIDTH 320
#define DISPLAY_HEIGHT 640



void load_program(State *state)
{
    state->square.x = 0;
    state->square.y = 0;
    state->square.w = 1 * DISPLAY_WIDTH / 16;
    state->square.h = 1 * DISPLAY_HEIGHT / 32;

    printf("Square width: %.0f, height: %.0f\n",
           state->square.w,
           state->square.h);

    // Initialize 512 "LEDs"
    int LED_ID = 0;
    float y = 0;
    // An LED is black by default
    int default_rgba = 0;
    for (int row = 0; row < 32; ++row)
    {
        float x = 0;
        for (int column = 0; column < 16; ++column)
        {
            state->LEDs[LED_ID].x = x;
            state->LEDs[LED_ID].y = y;
            state->LEDs[LED_ID].w = state->square.w;
            state->LEDs[LED_ID].h = state->square.h;

            // Color
            state->LEDs[LED_ID].r = default_rgba;
            state->LEDs[LED_ID].g = default_rgba;
            state->LEDs[LED_ID].b = default_rgba;
            state->LEDs[LED_ID].a = default_rgba;

            LED_ID += 1;
            x += state->square.w;
        }
        y += state->square.h;
    }

    // Initialize animation helper
    state->animator.current_frame = 1;
}

int listen_for_events(SDL_Window *window, State *state, float dt)
{
    void set_rgba(int LED_number, int r, int g, int b, int a, State *state);
    void set_rgba_random(int LED_number, State* state);

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
                            set_rgba_random(201, state);
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
    return;
}

void render(SDL_Renderer *renderer, State *state)
{
    // Draw BG
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Draw 512 LEDs
    for (int i = 0; i < 512; ++i)
    {
        SDL_SetRenderDrawColor(renderer, state->LEDs[i].r, state->LEDs[i].g, state->LEDs[i].b, state->LEDs[i].a);
        SDL_Rect LED_rect = {state->LEDs[i].x,
                             state->LEDs[i].y,
                             state->LEDs[i].w,
                             state->LEDs[i].h};
        SDL_RenderFillRect(renderer, &LED_rect);
    }

    // Present everything
    SDL_RenderPresent(renderer);
}


/*************
Animation
*************/

void set_rgba(int LED_number, int r, int g, int b, int a, State *state)
{
    // Set the r, g, b and a of a single LED
    state->LEDs[LED_number].r = r;
    state->LEDs[LED_number].g = g;
    state->LEDs[LED_number].b = b;
    state->LEDs[LED_number].a = a;

    printf("RGBA of LED %d: %d, %d, %d, %d\n", LED_number, r, g, b, a);
}

void set_rgba_random(int LED_number, State *state)
{
    void set_rgba(int LED_number, int r, int g, int b, int a, State *state);

    // Get random rgba values
    int rand_r = 255;
    int rand_g = 255;
    int rand_b = 255;
    int rand_a = 255;

    // Set the chosen LED's color to those values
    set_rgba(LED_number, rand_r, rand_g, rand_b, rand_a, state);
    printf("set_rgba_random(%d, %d, %d, %d, %d, state)\n", LED_number, rand_r, rand_g, rand_b, rand_a);
}

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
                              DISPLAY_WIDTH,                                  // Width
                              DISPLAY_HEIGHT,                                  // Height
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

        // Render stuff.
        render(renderer, &state);

        // Do everything according to delta time.
        time_1 = time_2;
        SDL_Delay(floor(16.666f - dt));
    }

    // Clean up
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    return 0;
}
