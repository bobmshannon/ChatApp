/*
* @Author: Robert Shannon <rshannon@buffalo.edu>
* @Date:   2016-02-02 20:13:26
* @Last Modified by:   Bobby
* @Last Modified time: 2016-02-06 18:56:10
*/

#include <signal.h>
#include <string>
#include <logger.h>
#include "../include/console.h"

Console::Console(void) {
    initscr();     // Start curses mode
    cbreak();      // Disable line buffering
    start_color(); // Enable color support
    init_pair(1, COLOR_WHITE, COLOR_BLUE);
    init_pair(2, COLOR_RED, COLOR_WHITE);

    // Chat cursor y-position
    chat_curs_y = CHAT_WINDOW_STARTY;

    // Setup chat and command windows
    chat_window = create_newwin(LINES - CMD_WINDOW_HEIGHT, COLS, 0, 0);
    cmd_window =
        create_newwin(CMD_WINDOW_HEIGHT, COLS, LINES - CMD_WINDOW_HEIGHT, 0);

    // Initial running state
    running = true;

    // Set window background colors
    wbkgd(chat_window, COLOR_PAIR(1));
    wbkgd(cmd_window, COLOR_PAIR(2));

    // Set initial window content
    mvwprintw(chat_window, CHAT_WINDOW_STARTY, CHAT_WINDOW_STARTX,
              CHAT_WINDOW_CONTENT);
    mvwprintw(cmd_window, CMD_WINDOW_STARTY, CMD_WINDOW_STARTX,
              CMD_WINDOW_CONTENT);
    wmove(cmd_window, CMD_WINDOW_STARTY, CMD_WINDOW_STARTX + 2);
    // Paint initial frame
    refresh();
}

Console::~Console(void) {
    running = false;
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
    wrefresh(local_win);  /* Show that box  */

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
    clearchat();          // Clear chat window
    print(cmd);           // Echo input
    wrefresh(cmd_window); // Refresh cmd window
    clearcmd();           // Clear cmd window
    read();               // Wait for more user input
}

void Console::print(std::string str) {
    if (chat_curs_y >= LINES - CMD_WINDOW_HEIGHT) {
        chat_curs_y = CHAT_WINDOW_STARTY;
        clearchat();
    }
    mvwprintw(chat_window, chat_curs_y, CHAT_WINDOW_STARTX,
              str.c_str()); // Print string to chat window
    wrefresh(chat_window);  // Refresh chat window
    cse4589_print_and_log(str.c_str());
    chat_curs_y += 1;
}

void Console::reset_curs() {
    wmove(cmd_window, CMD_WINDOW_STARTY,
          CMD_WINDOW_STARTX + 2); // Reset cursor to default position
    wrefresh(cmd_window);
}

void Console::clearchat() {
    werase(chat_window);    // Clear chat window
    box(chat_window, 0, 0); // Re-draw chat window borders
    wrefresh(chat_window);
}

void Console::clearcmd() {
    werase(cmd_window);    // Clear cmd window
    box(cmd_window, 0, 0); // Re-draw cmd window borders
    mvwprintw(cmd_window, CMD_WINDOW_STARTY, CMD_WINDOW_STARTX,
              CMD_WINDOW_CONTENT);
    reset_curs(); // Reset cursor position
}

void Console::refresh() {
    wrefresh(cmd_window);
    wrefresh(chat_window);
}

void Console::exit() { running = false; }

std::string Console::read() {
    char cmd[CMD_LENGTH];
    reset_curs();
    wgetstr(cmd_window, cmd);
    clearcmd();
    return std::string(cmd);
}
