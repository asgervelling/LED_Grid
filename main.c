#include <stdio.h>
#include <math.h>

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

    printf("Square width: %d, height: %d\n",
           state->square.w,
           state->square.h);

    // Initialize 512 "LEDs"
    int LED_ID = 0;
    long y = 0;
    for (int row = 0; row < 32; ++row)
    {
        long x = 0;
        for (int column = 0; column < 16; ++column)
        {
            state->LEDs[LED_ID].x = x;
            state->LEDs[LED_ID].y = y;
            state->LEDs[LED_ID].w = state->square.w;
            state->LEDs[LED_ID].h = state->square.h;

            LED_ID += 1;
            x += state->square.w;
        }
        y += state->square.h;
    }

    printf("LED 0's location: %d, %d", state->LEDs[0].x, state->LEDs[0].y);
}

int listen_for_events(SDL_Window *window, State *state, float dt)
{
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
    void animate_linear_function(float a, float x, float b, float dt);

    animate_linear_function(1, 2, 3, dt);
}

void render(SDL_Renderer *renderer, State *state)
{
    // Draw BG
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Draw 512 LEDs
    int g = 100;
    SDL_SetRenderDrawColor(renderer, 255, g, 255, 255);

    for (int i = 0; i < 20; ++i)
    {
        SDL_Rect LED_rect = {state->LEDs[i].x,
                             state->LEDs[i].y,
                             state->LEDs[i].x,
                             state->LEDs[i].h};
        SDL_RenderFillRect(renderer, &LED_rect);

        /*
        printf("LED %d:\n"
               "x: %d, y: %d, w: %d, h: %d\n\n",
               i,
               state->LEDs[i].x,
               state->LEDs[i].y,
               state->LEDs[i].w,
               state->LEDs[i].h);
        */

    }

    printf("LED 0: %d, %d %d %d\n", state->LEDs[0].x, state->LEDs[0].y, state->LEDs[0].w, state->LEDs[0].h);
    printf("LED 16: %d, %d %d %d\n", state->LEDs[16].x, state->LEDs[16].y, state->LEDs[16].w, state->LEDs[16].h);

    // Present everything
    SDL_RenderPresent(renderer);
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
    window = SDL_CreateWindow("Game Window",                        // Window title
                              SDL_WINDOWPOS_UNDEFINED,              // Initial x position
                              SDL_WINDOWPOS_UNDEFINED,              // Initial y position
                              DISPLAY_WIDTH,                                  // Width
                              DISPLAY_HEIGHT,                                  // Height
                              0);                                   // Flags

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    state.renderer = renderer;



    // Event loop
    int done = 0;

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
