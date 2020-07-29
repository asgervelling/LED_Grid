#include <stdio.h>
#include <string.h>
#include <stddef.h>

#include "structs.h"
#include "graphics.h"

void file_write_test()
{
    FILE *f;

    f = fopen("animations/animation_1.txt", "w");
    fputc('d', f);
    fclose(f);
}

void append_LED_to_file(char filename[], int frame, int row, int column, int r, int g, int b, int a)
{
    char path[100] = "animations/";
    strcat(path, filename);
    strcat(path, ".txt");

    FILE *f;

    f = fopen(path, "a");
    fprintf(f, "frame: %d row: %d column: %d R: %d G: %d B: %d A: %d\n", frame, row, column, r, g, b, a);
    fclose(f);
}

void save_animation(State *state, char filename[])
{
    // Use "state" to gather information about the current user animation.
    // Save the animation at "animations/<filename>.txt".
    char path[100] = "animations/";
    strcat(path, filename);
    strcat(path, ".txt");

    FILE *f;

    // Delete contents of file, if the file exists
    f = fopen(path, "w");
    fclose(f);

    // Append all frames' info to a text file
    for (int frame = 0; frame < 32; ++frame)
    {
        for (int row = 0; row < 32; ++row)
        {
            for (int column = 0; column < 16; ++column)
            {
                
                append_LED_to_file(filename,
                                frame,
                                row,
                                column,
                                state->animation.user_animation.frames[frame].LEDs[row][column].r,
                                state->animation.user_animation.frames[frame].LEDs[row][column].g,
                                state->animation.user_animation.frames[frame].LEDs[row][column].b,
                                state->animation.user_animation.frames[frame].LEDs[row][column].a);
            }
        }
    }
}

void load_LED_from_file(State *state, int frame, int row, int col, int r, int g, int b, int a)
{
    state->animation.user_animation.frames[frame].LEDs[row][col].r = r;
    state->animation.user_animation.frames[frame].LEDs[row][col].g = g;
    state->animation.user_animation.frames[frame].LEDs[row][col].b = b;
    state->animation.user_animation.frames[frame].LEDs[row][col].a = a;
}

void test_that_loading_works(State *state)
{
    for (int i = 0; i < 15; ++i)
    {
        printf("From frame 4, row 4: %d %d %d %d\n", state->animation.user_animation.frames[4].LEDs[3][i].r,
                                                    state->animation.user_animation.frames[4].LEDs[3][i].g,
                                                    state->animation.user_animation.frames[4].LEDs[3][i].b,
                                                    state->animation.user_animation.frames[4].LEDs[3][i].a);
    }
}

void load_animation_from_file(State *state, char path[])
{
    FILE *f = fopen(path, "r");
    
    if(f == NULL)
    {
        perror("Error in opening file\n");
        return;
    }

    int frame, row, column, r, g, b, a;
    while(fscanf(f, "frame: %d row: %d column: %d R: %d G: %d B: %d A: %d\n", &frame, &row, &column, &r, &g, &b, &a) != EOF)
    {
        load_LED_from_file(state, frame, row, column, r, g, b, a);
    }
    
    test_that_loading_works(state);

    // Display the currently active frame
    show_animation_frame(state);

    fclose(f);
}

