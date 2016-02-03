#ifndef CONSOLE_H_
#define CONSOLE_H_

#define PROMPT_HEIGHT 3
/**
 * http://tldp.org/HOWTO/NCURSES-Programming-HOWTO/windows.html
 */
WINDOW *create_newwin(int height, int width, int starty, int startx);
void destroy_win(WINDOW *local_win);

#endif