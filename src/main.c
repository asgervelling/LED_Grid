#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "headers.h"
#include "graphics.h"
#include "file_io.h"

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

void load_program(State *state, SDL_Renderer *renderer)
{
    init_settings(state);
    init_square(state);
    init_LEDs(state);
    init_animation(state);
    init_GUI(state, renderer);
    init_user_tools(state);
    init_new_animation(state, 32);
}

/*************
Control
*************/

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

void click_LED(State *state)
{
    // This function lets you hold down the left mouse button
    if (state->animation.user_animation.animation_mode == 0)
    {
        return;
    }

    // User should only be able to use this function when clicking the LEDs.
    if (mouse_hovering(state, state->GUI.x,
                              state->GUI.y,
                              state->GUI.w,
                              state->GUI.h))
    {
        return;
    }

    state->animation.user_animation.active_color[0] = 255;
    state->animation.user_animation.active_color[1] = 0;
    state->animation.user_animation.active_color[2] = 0;
    state->animation.user_animation.active_color[3] = 255;
    // Flip x and y
    int LED_row = state->mouse.y / state->settings.LED_width;
    int LED_col = state->mouse.x / state->settings.LED_height;
    set_rgba(LED_row,
             LED_col,
             state->animation.user_animation.active_color[0],
             state->animation.user_animation.active_color[1],
             state->animation.user_animation.active_color[2],
             state->animation.user_animation.active_color[3],
             state);
    store_single_LED(state,
                     LED_row,
                     LED_col,
                     state->animation.user_animation.active_frame);
}

int button_pressed(State *state, Button button)
{
    if (mouse_hovering(state,
                       button.x,
                       button.y,
                       button.w,
                       button.h))
    {
        return 1;
    }
    return 0;
}



void click(State *state)
{
    void do_test_animation(State *state);
    int mouse_hovering(State *state, int bx, int by, int bw, int bh);
    void start_animation(State *state, int animation_enum, int animation_object);
    void stop_animation(State *state, int clear_screen);
    void click_LED(State *state);

    // Stop animation button
    /*
    if (mouse_hovering(state, state->GUI.button_stop.x,
                              state->GUI.button_stop.y,
                              state->GUI.button_stop.w,
                              state->GUI.button_stop.h))
    {
        stop_animation(state, 1);
    }
    */

    if (button_pressed(state, state->GUI.button_stop))
    {
        stop_animation(state, 1);
    }

    // "Animation 1"-button
    if (button_pressed(state, state->GUI.button_animation_1))
    {
        stop_animation(state, 1);
        init_LEDs(state);
        load_animation_from_file(state, "animations/this_is_a_file.txt");
    }

    // Animation 2 button
    if (button_pressed(state, state->GUI.button_animation_2))
    {
        stop_animation(state, 1);
        start_animation(state, gradient_anim, LED_anim);
    }

    // New animation button
    if (button_pressed(state, state->GUI.button_new_animation))
    {
        if (state->animation.user_animation.animation_mode == 0)
        {
            state->animation.user_animation.animation_mode = 1;
        }
        else
        {
            state->animation.user_animation.animation_mode = 0;
        }
    }

    // Save animation button
    if (button_pressed(state, state->GUI.button_save_animation))
    {
        save_animation(state, "this_is_a_file");
    }

    // Timeline buttons
    if (state->animation.user_animation.animation_mode == 1)
    {
        for (int i = 0; i < 32; ++i)
        {
            if (button_pressed(state, state->GUI.button_animation_frame[i]))
            {
                state->animation.user_animation.active_frame = i;
                show_animation_frame(state);
            }
        }
    }

    // Play button
    if (button_pressed(state, state->GUI.button_play))
    {
        stop_animation(state, 1);
        start_animation(state, user_animation, LED_anim);
    }

}

void on_hover(State *state)
{
    // Test dropdown menu
    if (mouse_hovering(state,
                       state->GUI.test_dropdown.x,
                       state->GUI.test_dropdown.y,
                       state->GUI.test_dropdown.w,
                       state->GUI.test_dropdown.h))
    {
        start_animation(state, dropdown_animation_in, GUI_anim);
    }
    else
    {
        start_animation(state, dropdown_animation_out, GUI_anim);
    }
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
                            start_animation(state, test_animation, LED_anim);
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

    // Mouse state
    SDL_PumpEvents();
    if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
        click_LED(state);
    }
    

    return done;
}

void process(State *state, float dt)
{
    // get mouse position
    SDL_GetMouseState(&state->mouse.x, &state->mouse.y);

    // Animation
    do_current_animation(state, state->animation.current_animation);
    do_GUI_animation(state, state->GUI_animation.current_animation);

    printf("LED Anim frame: %d\nGUI Anim frame: %d\n\n", state->animation.current_frame, state->GUI_animation.current_frame);

    on_hover(state);
}

void render(SDL_Renderer *renderer, State *state)
{
    void render_GUI(State *state, SDL_Renderer *renderer);
    render_GUI(state, renderer);
    // Present everything
    SDL_RenderPresent(renderer);
}

void render_GUI(State *state, SDL_Renderer *renderer)
{
    /**********************
    Render the entire GUI. Most of the buttons are just rectangles for now, adding images later.
    **********************/
    // Colors
    int blue[16] = {0, 80, 180, 255};
    int dark_blue[16] = {0, 40, 90, 255};
    int light_gray[16] = {210, 210, 255, 255};

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

    /**************
    Main GUI
    **************/

    // Draw GUI element
    SDL_SetRenderDrawColor(renderer, blue[0], blue[1], blue[2], blue[3]);
    SDL_Rect GUI_rect = {state->GUI.x,
                         state->GUI.y,
                         state->GUI.w,
                         state->GUI.h};
    SDL_RenderFillRect(renderer, &GUI_rect);

    /* Buttons */

    // Test dropdown
    SDL_SetRenderDrawColor(renderer, light_gray[0], light_gray[1], light_gray[2], light_gray[3]);
    SDL_Rect dropdown_rect = {state->GUI.test_dropdown.x,
                              state->GUI.test_dropdown.y,
                              state->GUI.test_dropdown.w,
                              state->GUI.test_dropdown.h};
    SDL_RenderFillRect(renderer, &dropdown_rect);

    // Stop button
    SDL_SetRenderDrawColor(renderer, 150, 0, 0, 255);
    SDL_Rect stop_button_rect = {state->GUI.button_stop.x,
                                 state->GUI.button_stop.y,
                                 state->GUI.button_stop.w,
                                 state->GUI.button_stop.h};
    SDL_RenderFillRect(renderer, &stop_button_rect);

    // Animation 1 button
    SDL_SetRenderDrawColor(renderer, light_gray[0], light_gray[1], light_gray[2], light_gray[3]);
    SDL_Rect anim_1_button_rect = {state->GUI.button_animation_1.x,
                                   state->GUI.button_animation_1.y,
                                   state->GUI.button_animation_1.w,
                                   state->GUI.button_animation_1.h};
    SDL_RenderCopy(renderer, state->GUI.button_animation_1.image_texture, NULL, &anim_1_button_rect);
    

    // Randomize button
    SDL_Rect rand_button_rect = {state->GUI.button_animation_2.x,
                                 state->GUI.button_animation_2.y,
                                 state->GUI.button_animation_2.w,
                                 state->GUI.button_animation_2.h};
    SDL_RenderFillRect(renderer, &rand_button_rect);

    /**************
    User animations GUI element
    **************/

    SDL_SetRenderDrawColor(renderer, dark_blue[0], dark_blue[1], dark_blue[2], dark_blue[3]);
    SDL_Rect GUI_elem_user_anim_rect = {state->GUI.GUI_elem_user_animation.x,
                                        state->GUI.GUI_elem_user_animation.y,
                                        state->GUI.GUI_elem_user_animation.w,
                                        state->GUI.GUI_elem_user_animation.h};
    SDL_RenderFillRect(renderer, &GUI_elem_user_anim_rect);

    // Buttons

    // New animation button
    if (state->animation.user_animation.animation_mode == 1)
    {
        SDL_SetRenderDrawColor(renderer, 120, 120, 120, 255);
    }
    else
    {
        SDL_SetRenderDrawColor(renderer, light_gray[0], light_gray[1], light_gray[2], light_gray[3]);
    }
    
    SDL_Rect button_new_anim_rect = {state->GUI.button_new_animation.x,
                                     state->GUI.button_new_animation.y,
                                     state->GUI.button_new_animation.w,
                                     state->GUI.button_new_animation.h};
    SDL_RenderFillRect(renderer, &button_new_anim_rect);

    SDL_SetRenderDrawColor(renderer, light_gray[0], light_gray[1], light_gray[2], light_gray[3]);
    // Save animation button
    SDL_Rect button_save_anim_rect = {state->GUI.button_save_animation.x,
                                      state->GUI.button_save_animation.y,
                                      state->GUI.button_save_animation.w,
                                      state->GUI.button_save_animation.h};
    SDL_RenderFillRect(renderer, &button_save_anim_rect);

    // Timeline:
    // Consists of n buttons that lead you to a specific frame in the user animation when clicked
    int max_frames = 32;
    for (int i = 0; i < max_frames; ++i)
    {
        SDL_Rect button_frame_rect = {state->GUI.button_animation_frame[i].x,
                                      state->GUI.button_animation_frame[i].y,
                                      state->GUI.button_animation_frame[i].w,
                                      state->GUI.button_animation_frame[i].h};
        // Highlight active frame
        if (state->animation.user_animation.active_frame == i)
        {
            SDL_SetRenderDrawColor(renderer, 120, 120, 120, 255);
        }
        else
        {
            SDL_SetRenderDrawColor(renderer, light_gray[0], light_gray[1], light_gray[2], light_gray[3]);
        }
        SDL_RenderFillRect(renderer, &button_frame_rect);
    }

    // Play button
    SDL_Rect dest = {state->GUI.button_play.x,
                     state->GUI.button_play.y,
                     state->GUI.button_play.w,
                     state->GUI.button_play.h};
    SDL_RenderCopy(renderer,
                   state->GUI.button_play.image_texture,
                   NULL,
                   &dest);
}

/*************
Main
*************/

int main(int argc, char* argv[])
{
    // Declarations
    void load_program(State *state, SDL_Renderer *renderer);
    int listen_for_events(SDL_Window *window, State *state, float dt);
    void process(State *state, float dt);
    void render(SDL_Renderer *renderer, State *state);

    // Declare a state. This is used for interacting with EVERYTHING.
    State state;

    // Initialize
    SDL_Init(SDL_INIT_EVERYTHING);
    // load support for the JPG and PNG image formats
    int flags = IMG_INIT_JPG | IMG_INIT_PNG;
    int initted = IMG_Init(flags);
    if ((initted & flags) != flags) 
    {
        printf("IMG_Init: Failed to init required jpg and png support!\n");
        printf("IMG_Init: %s\n", IMG_GetError());
        // handle error
    }

    // Declare SDL objects
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    // Create window.
    // I made the window bigger than 16x32, but kept that aspect ratio for the LED GUI.
    window = SDL_CreateWindow("LEDs",                        // Window title
                              SDL_WINDOWPOS_UNDEFINED,              // Initial x position
                              SDL_WINDOWPOS_UNDEFINED,              // Initial y position
                              960,
                              640,
                              0);                                   // Flags

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    state.renderer = renderer;

    // Initialize most things in the program
    load_program(&state, renderer);    

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
    SDL_DestroyTexture(state.GUI.button_animation_1.image_texture);
    SDL_DestroyTexture(state.GUI.button_play.image_texture);
    IMG_Quit();
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    return 0;
}
