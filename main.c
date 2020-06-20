#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "SDL.h"
#include "headers/structs.h"
#include "headers/file_io.h"

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
    void init_square(State *state);
    void init_LEDs(State *state);
    void init_animation(State *state);
    void init_GUI(State *state);

    init_square(state);
    init_LEDs(state);
    init_animation(state);
    init_GUI(state);
}

// Just some initializing functions
void init_LEDs(State *state)
{
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
}

void init_animation(State *state)
{
    // Initialize animation helper
    state->animation.playing = 0;
    state->animation.current_frame = 0;
    state->animation.current_animation = no_animation;
}

void init_square(State *state)
{
    state->square.x = 0;
    state->square.y = 0;
    state->square.w = 1 * BRUTUS_WIDTH / 16;
    state->square.h = 1 * BRUTUS_HEIGHT / 32;
}

void init_GUI(State *state)
{
    SDL_Surface* IMG_Load(const char *file);

    // Button image
    SDL_Surface *surface = IMG_Load("assets/GUI/button_randomize.png");
    img_load_error_msg(surface, "button_randomize.png");
    state->GUI.randomize_button.texture = SDL_CreateTextureFromSurface(state->renderer, surface);
    SDL_FreeSurface(surface);

    // GUI rect
    state->GUI.x = BRUTUS_WIDTH;
    state->GUI.y = 0;
    state->GUI.w = BRUTUS_WIDTH;
    state->GUI.h = DISPLAY_HEIGHT;
    printf("GUI Y: %d\n", state->GUI.y);

    // Button rect
    state->GUI.randomize_button.x = state->GUI.x + 40;
    state->GUI.randomize_button.y = state->GUI.y + 40;
    state->GUI.randomize_button.w = 120;
    state->GUI.randomize_button.h = 34;
}

int listen_for_events(SDL_Window *window, State *state, float dt)
{
    void set_rgba_random_all_LEDs(State *state);
    void func_test_animation(State *state, int length_in_frames);
    void click(State *state);
    void start_animation(State *state, int animation_enum);

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

    // Animation
    void do_current_animation(State *state, int animation);

    do_current_animation(state, test_animation);
}

void render(SDL_Renderer *renderer, State *state)
{
    // Draw BG
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    // SDL_RenderClear(renderer);

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

    // Draw GUI
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_Rect GUI_rect = {state->GUI.x,
                         state->GUI.y,
                         state->GUI.w,
                         state->GUI.h};
    SDL_RenderFillRect(renderer, &GUI_rect);

    // Draw button
    SDL_SetRenderDrawColor(renderer, 210, 210, 210, 255);
    SDL_Rect rand_button_rect = {state->GUI.randomize_button.x,
                                 state->GUI.randomize_button.y,
                                 state->GUI.randomize_button.w,
                                 state->GUI.randomize_button.h};

    printf("GUI:\n");
    printf("x: %d, y: %d, w: %d, h: %d\n", GUI_rect.x, GUI_rect.y, GUI_rect.w, GUI_rect.h);
    printf("Button:\n");
    printf("x: %d, y: %d, w: %d, h: %d\n\n", rand_button_rect.x, rand_button_rect.y, rand_button_rect.w, rand_button_rect.h);

    // SDL_RenderCopy(state->renderer, state->GUI.randomize_button.texture, NULL, &rand_button_rect);
    SDL_RenderFillRect(state->renderer, &rand_button_rect);
    // printf("width: %d\n", state->GUI.randomize_button.w);
    // Present everything
    SDL_RenderPresent(renderer);
}

/*************
Animation
*************/

void start_animation(State *state, int animation_enum)
{
    state->animation.playing = 1;
    state->animation.current_animation = animation_enum;
}

void set_rgba(int LED_number, int r, int g, int b, int a, State *state)
{
    // Set the r, g, b and a of a single LED
    state->LEDs[LED_number].r = r;
    state->LEDs[LED_number].g = g;
    state->LEDs[LED_number].b = b;
    state->LEDs[LED_number].a = a;
}

void set_rgba_random(int LED_number, State *state)
{
    void set_rgba(int LED_number, int r, int g, int b, int a, State *state);

    // Get random rgba values
    int rand_r = rand() % 256;
    int rand_g = rand() % 256;
    int rand_b = rand() % 256;
    int rand_a = rand() % 256;

    // Set the chosen LED's color to those values
    set_rgba(LED_number, rand_r, rand_g, rand_b, rand_a, state);
}

void set_rgba_random_all_LEDs(State* state)
{
    void set_rgba_random(int LED_number, State* state);
    for (int i = 0; i < 512; ++i)
    {
        set_rgba_random(i, state);
    }
}

void do_test_animation(State *state)
{
    void set_rgba(int LED_number, int r, int g, int b, int a, State *state);
    void set_default_all_LEDs(State *state);

    set_default_all_LEDs(state);
    set_rgba(state->animation.animation_frame, 0, 255, 0, 255, state);
}

void set_default_all_LEDs(State *state)
{
    void set_rgba(int LED_number, int r, int g, int b, int a, State *state);

    // Set the color of all LEDs to black
    for (int i = 0; i < LED_NO; ++i)
    {
        set_rgba(i, 0, 0, 0, 255, state);
    }
}

void do_current_animation(State *state, int animation) // Called every frame
{
    void do_test_animation(State *state);

    if (state->animation.playing == 0)
    {
        state->animation.animation_frame = 0;
        state->animation.current_frame = 0;
        return;
    }

    // Set the speed of the animation
    int frame_length_ms = 2;
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
        do_test_animation(state);
    }

}

/*************
Control
*************/

void click(State *state)
{
    void do_test_animation(State *state);

    // "Animation 1"-button
    if (state->mouse.x >= state->GUI.randomize_button.x &&
        state->mouse.x <= state->GUI.randomize_button.x + state->GUI.randomize_button.w &&
        state->mouse.y >= state->GUI.randomize_button.y &&
        state->mouse.y <= state->GUI.randomize_button.y + state->GUI.randomize_button.h)
    {
        start_animation(state, test_animation);
    }
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
                              DISPLAY_WIDTH,
                              DISPLAY_HEIGHT,
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
        SDL_Delay(floor(16.666f - dt));
    }

    // Clean up
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    return 0;
}
