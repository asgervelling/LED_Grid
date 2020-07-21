#ifndef FILE_IO
#define FILE_IO

void file_write_test();
void append_LED_to_file(char filename[], int r, int g, int b, int a);
void save_animation(State *state, char filename[]);
void load_LED_from_file(State *state, int frame, int row, int col, int r, int g, int b, int a);
void play_animation_from_file(State *state, char filename[]);

#endif