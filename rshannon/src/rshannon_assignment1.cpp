/**
* @ChatApp
* @author  Robert Shannon <rshannon@buffalo.edu>
* @version 1.0
*
* @section LICENSE
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License as
* published by the Free Software Foundation; either version 2 of
* the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
* General Public License for more details at
* http://www.gnu.org/copyleft/gpl.html
*
* @section DESCRIPTION
*
* This contains the main function. Add further description here....
*/
#include <iostream>
#include <stdio.h>
#include <curses.h>

#include "../include/global.h"
#include "../include/logger.h"
#include "../include/console.h"

using namespace std;

/**
* main function
* @param  argc Number of arguments
* @param  argv The argument list
* @return 0 EXIT_SUCCESS
*/
int main(int argc, char** argv) {
    // Initialize Logger
    cse4589_init_log(argv[2]);

    // Clear LOGFILE
    fclose(fopen(LOGFILE, "w"));

    // Fetch user input
    WINDOW *chat_window, *cmd_window;
    int ch;
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
    wbkgd(chat_window, COLOR_PAIR(1));
    wbkgd(cmd_window, COLOR_PAIR(2));
    mvwprintw(cmd_window, CMD_WINDOW_STARTX, CMD_WINDOW_STARTY, "Enter a command...");
    wmove(cmd_window, CMD_WINDOW_STARTX, CMD_WINDOW_STARTY);
    wrefresh(chat_window);
    wrefresh(cmd_window);
    wgetstr(cmd_window, cmd);

    while ((ch = getch()) != KEY_F(1)) {
    }

    endwin();
}