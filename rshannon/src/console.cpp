/*
* @Author: Robert Shannon <rshannon@buffalo.edu>
* @Date:   2016-02-02 20:13:26
* @Last Modified by:   Bobby
* @Last Modified time: 2016-02-03 00:00:29
*/

#include <ncurses.h>
#include "../include/console.h"

Console::Console(void) {
    char cmd[CMD_LENGTH];

    initscr(); /* Start curses mode */
    cbreak();  /* Disable line buffering */
    refresh(); /* Paint initial frame */
    start_color(); /* Enable color support */
    init_pair(1, COLOR_WHITE, COLOR_BLUE);
    init_pair(2, COLOR_RED, COLOR_WHITE);

    // Setup chat and command windows
    chat_window = create_newwin(LINES - CMD_WINDOW_HEIGHT, COLS, 0, 0);
    cmd_window = create_newwin(CMD_WINDOW_HEIGHT, COLS, LINES - CMD_WINDOW_HEIGHT, 0);

    // Set window background colors
    wbkgd(chat_window, COLOR_PAIR(1));
    wbkgd(cmd_window, COLOR_PAIR(2));

    // Set initial window content
    mvwprintw(chat_window, CHAT_WINDOW_STARTY, CHAT_WINDOW_STARTX, CHAT_WINDOW_CONTENT);
    mvwprintw(cmd_window, CMD_WINDOW_STARTY, CMD_WINDOW_STARTX, CMD_WINDOW_CONTENT);
    wmove(cmd_window, CMD_WINDOW_STARTY, CMD_WINDOW_STARTX + 2);
    wrefresh(chat_window);
    wrefresh(cmd_window);

    // Wait for user input
    wgetstr(cmd_window, cmd);
    process_command(cmd);
    mvwprintw(chat_window, CHAT_WINDOW_STARTY, CHAT_WINDOW_STARTX, cmd);
    wrefresh(chat_window);
    wrefresh(cmd_window);
}

Console::~Console(void) {
	destroy_win(cmd_window);
	destroy_win(chat_window);
	endwin();
}

WINDOW* Console::create_newwin(int height, int width, int starty, int startx) {
    WINDOW* local_win;

    local_win = newwin(height, width, starty, startx);
    box(local_win, 0, 0); /* 0, 0 gives default characters
                           * for the vertical and horizontal
                           * lines */
    wrefresh(local_win);  /* Show that box 	*/

    return local_win;
}

void Console::destroy_win(WINDOW* local_win) {
    /* box(local_win, ' ', ' '); : This won't produce the desired
     * result of erasing the window. It will leave it's four corners
     * and so an ugly remnant of window.
     */
    wborder(local_win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    /* The parameters taken are
     * 1. win: the window on which to operate
     * 2. ls: character to be used for the left side of the window
     * 3. rs: character to be used for the right side of the window
     * 4. ts: character to be used for the top side of the window
     * 5. bs: character to be used for the bottom side of the window
     * 6. tl: character to be used for the top left corner of the window
     * 7. tr: character to be used for the top right corner of the window
     * 8. bl: character to be used for the bottom left corner of the window
     * 9. br: character to be used for the bottom right corner of the window
     */
    wrefresh(local_win);
    delwin(local_win);
}

void Console::process_command(char cmd[]) {
	return;
}
void Console::chatprint() {
	return;
}
void Console::clearchat() {
	return;
}
void Console::clearcmd() {
	return;
}
void Console::refresh() {
	return;
}