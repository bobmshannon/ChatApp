#ifndef CONSOLE_H_
#define CONSOLE_H_

#define CMD_WINDOW_HEIGHT 3
#define CMD_WINDOW_STARTX 1
#define CMD_WINDOW_STARTY 1
#define CMD_LENGTH 1024

/**
 * http://tldp.org/HOWTO/NCURSES-Programming-HOWTO/windows.html
 */
WINDOW *create_newwin(int height, int width, int starty, int startx);
void destroy_win(WINDOW *local_win);

#endif