#include <stdio.h>
#include <stdlib.h>

#define CANVAS_SIZE_X 32
#define CANVAS_SIZE_Y 32

char read_char_from_file(char *filename, int line_no, int char_no)
{
    FILE *fp = fopen(filename, "r");
    char line[150];
    int line_number = 0;
    char desired_char;

    int running = 1;
    while (!feof(fp) && running == 1)
    {
        fgets(line, 150, fp);
        if (line_number == line_no)
        {
            running = 0;
        }
        line_number += 1;
    }

    desired_char = line[char_no];
    fclose(fp);
    return desired_char;
}

int length(char *arr)
{
    int length = 0;
    int running = 1;
    while (running == 1)
    {
        if (arr[length] != EOF && arr[length] != '\0')
        {
            ++length;
        }
        else
        {
            running = 0;
        }
    }
    return length;
}


/*

Load images

*/

void img_load_error_msg(SDL_Surface *surf, char* filename)
{
    if (surf == NULL)
    {
        printf("\nCannot load surface:\t");
        for (int j = 0; j < length(filename); ++j)
        {
            printf("%c", filename[j]);
        }
        SDL_Quit();
        exit(1);
    }
}

void get_file_name(char *char_array, int ascii_int, int insert_index)
{
    char c = ascii_int;
    c = ascii_int;
    char_array[insert_index] = c;
}

void get_longer_file_name(char *char_array, int ascii_int_1, int ascii_int_2, int insert_index)
{
    char c1 = ascii_int_1;
    char c2 = ascii_int_2;
    char_array[insert_index] = c1;
    char_array[insert_index + 1] = c2;
}
