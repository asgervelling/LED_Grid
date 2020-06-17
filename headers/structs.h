typedef struct
{
    int x, y, w, h;
} Square;

typedef struct
{
    // This is an LED. There are 512 LED's in Brutus,
    // laid out in a 16x32 pattern.
    // When testing on the computer, an LED is represented by a square.
    int x, y, w, h;
} LED;

// State: Contains everything.
typedef struct
{
    Square square;
    LED LEDs[512];
    SDL_Renderer *renderer;
} State;
