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
    int current_frame;
} Animator;

// State: Contains everything.
typedef struct
{
    Square square;
    LED LEDs[512];
    SDL_Renderer *renderer;

    // Animation
    Animator animator;
} State;
