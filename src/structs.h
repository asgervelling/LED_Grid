#ifndef STRUCTS
#define STRUCTS

#include <SDL2/SDL.h>


typedef struct
{
    float x, y, w, h;
} Square;

typedef struct
{
    // This is an LED. There are 512 LED's in Brutus,
    // laid out in a 16x32 pattern.
    // When testing on the computer, an LED is represented by a square.

    float x, y, w, h;
    int r, g, b, a;
} LED;

// Animation helpers
enum animations
{
    // Until I learn to just do a hashtable lol
    no_animation,
    test_animation,
    ax_plus_b_anim,
    gradient_anim
};

// Controls
typedef struct
{
    int x, y;
} Mouse;

typedef struct
{
    SDL_Texture *texture;
    int x, y, w, h;
} Button;

typedef struct
{
    float x, y, w, h;

    // Buttons
    Button stop_button;
    Button randomize_button;
    Button animation_1_button;
} GUI_Element;

typedef struct
{
    int display_width, display_height;
    int brutus_width, brutus_height;
    int LED_width, LED_height;
} Settings;


typedef struct
{
    int playing;

    // Actual frame
    int current_frame;

    // Frame of an animation
    int animation_frame;
    int current_animation;

    float frame_rate;
} Animation;

// State: Contains everything.
typedef struct
{
    Square square;
    LED LEDs[32][16];
    LED LEDs_Transposed[16][32];
    SDL_Renderer *renderer;

    // Animation
    Animation animation;

    // GUI
    GUI_Element GUI;
    Mouse mouse;

    // Settings
    Settings settings;
} State;

#endif