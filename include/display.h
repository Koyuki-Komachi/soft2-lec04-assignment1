#ifndef DISPLAY
#define DISPLAY

// display functions
void rewind_screen(unsigned int line); 
void clear_command(void);
void clear_screen(void);
void draw_line(Canvas *c, const int x0, const int y0, const int x1, const int y1);

#endif