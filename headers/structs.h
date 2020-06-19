#include "SDL.h"

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

typedef struct
{
    SDL_Texture *texture;
    Square rect;
} Button;

typedef struct
{
    float x, y, w, h;
    Button randomize_button;
} GUI_Element;

// Animation helpers
enum animations
{
    // Until I learn to just do a hashtable lol
    no_animation,
    test_animation
};

typedef struct
{
    int playing;

    // Actual frame
    int current_frame;

    // Frame of an animation
    int animation_frame;
    int current_animation;
} Animation;

// State: Contains everything.
typedef struct
{
    Square square;
    LED LEDs[512];
    SDL_Renderer *renderer;

    // Animation
    Animation animation;

    // GUI
    GUI_Element GUI;
} State;
