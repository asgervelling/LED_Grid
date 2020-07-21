#ifndef STRUCTS
#define STRUCTS

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


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

// Controls
typedef struct
{
    int x, y;
} Mouse;

typedef struct
{
    int x, y, w, h;
    SDL_Texture *image_texture;
} Button;

typedef struct
{
    float x, y, w, h;

    // Buttons
    Button button_stop;
    Button button_animation_1;
    Button button_animation_2;

    // User animations
    Square GUI_elem_user_animation;
    Button button_new_animation;
    Button button_save_animation;

    // User timeline
    Button button_animation_frame[32]; // Timeline
} GUI_Element;

typedef struct
{
    int display_width, display_height;
    int brutus_width, brutus_height;
    int LED_GUI_width, LED_GUI_height;
    int LED_width, LED_height;
} Settings;

typedef struct
{
    int active_tool; // Enum
    int active_color[16];
} Tools;

enum tools
{
    tool_none,
    tool_paint_brush,
    tool_eraser,
    tool_color_picker
};

typedef struct
{
    LED LEDs[32][16];
} Animation_Frame;

typedef struct
{
    int animation_mode;
    int active_frame;
    int active_color[16];
    Animation_Frame frames[32];
} User_Animation;

typedef struct
{
    int playing;

    // Actual frame
    int current_frame;

    // Frame of an animation
    int animation_frame;
    int current_animation;

    float frame_rate;

    // User
    User_Animation user_animation;
} Animation;

// Animation helpers
enum animations
{
    // Some pre-made animation
    no_animation,
    test_animation,
    ax_plus_b_anim,
    gradient_anim
};

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

    // User tools
    Tools user_tools;

    // Settings
    Settings settings;
} State;

#endif