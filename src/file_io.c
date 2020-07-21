#include <stdio.h>
#include <string.h>
#include <stddef.h>

#include "structs.h"

void file_write_test()
{
    FILE *f;

    f = fopen("animations/animation_1.txt", "w");
    fputc('d', f);
    fclose(f);
}

void append_LED_to_file(char filename[], int r, int g, int b, int a)
{
    // Filename max 50 characters
    char path[100] = "animations/";
    strcat(path, filename);
    strcat(path, ".txt");

    FILE *f;

    f = fopen(path, "a");
    fprintf(f, "(%d, %d, %d, %d)\n", r, g, b, a);
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
        f = fopen(path, "a");
        fputs(f, "f\n");
        fclose(f);
        for (int row = 0; row < 32; ++row)
        {
            f = fopen(path, "a");
            fprintf(f, "r");
            fclose(f);
            for (int column = 0; column < 16; ++column)
            {
                
                append_LED_to_file(filename,
                                state->animation.user_animation.frames[frame].LEDs[row][column].r,
                                state->animation.user_animation.frames[frame].LEDs[row][column].g,
                                state->animation.user_animation.frames[frame].LEDs[row][column].b,
                                state->animation.user_animation.frames[frame].LEDs[row][column].a);
            }
            f = fopen(path, "a");
            fputs("\n", f);
            fclose(f);
        }
        f = fopen(path, "a");
        fputs("\n", f);
        fclose(f);
    }
    f = fopen(path, "a");
    // File terminator
    fputs("x", f);
    fclose(f);
}

void load_LED_from_file(State *state, int frame, int row, int col, int r, int g, int b, int a)
{
    state->animation.user_animation.frames[frame].LEDs[row][col].r = r;
    state->animation.user_animation.frames[frame].LEDs[row][col].g = g;
    state->animation.user_animation.frames[frame].LEDs[row][col].b = b;
    state->animation.user_animation.frames[frame].LEDs[row][col].a = a;
}

void play_animation_from_file(State *state, char path[])
{
    // Read a monstrous text file
    FILE *f = fopen(path, "r");
    char *buffer;
    long numbytes;

    if(f == NULL)
    {
        perror("Error in opening file\n");
        return;
    }
    
    fseek(f, 0L, SEEK_END);
    numbytes = ftell(f);
    fseek(f, 0L, SEEK_SET);
    
    buffer = (char*)calloc(numbytes, sizeof(char));
    if(buffer == NULL)
    {
        printf("Memory error\n");
    }
    fread(buffer, sizeof(char), numbytes, f);
    fclose(f);

    // Test read
    // printf("The file contains this text\n\n%s\n", buffer);
    

    // Parse the text
    char c;
    int i = 0;
    size_t length = strlen(buffer);

    int frame = 0;
    int row = 0;
    int column = 0;
    int reading_rgba = 0;
    int rgba_index = 0;
    int rgba[16];
    int rgba_buffer[4];
    while(1)
    {
        c = buffer[i];
        // Get frame, row and column number
        if (c == 'f')
        {
            ++frame;
        }
        if (c == 'r')
        {
            ++row;
        }
        if (c == '(')
        {
            ++column;
            int j = 0;
            while (c != ')')
            {
                if (c == ',')
                {
                    // CONTINUE FROM HERE
                    if (rgba_index == 0)
                    {
                        rgba_index = 3;
                    }
                    ++rgba_index;
                }
                else
                {
                    printf("the number is %c\n", c);
                }

                ++i;
                c = buffer[i];
                
            }
        }
        
        
        

        ++i;
        if (i > length)
        {
            break;
        }
    }
    free(buffer);
}