#ifndef CONSOLE_H_
#define CONSOLE_H_

#define CMD_WINDOW_HEIGHT 3
#define CMD_WINDOW_STARTX 1
#define CMD_WINDOW_STARTY 1
#define CMD_LENGTH 1024
#define CMD_WINDOW_CONTENT ">"

#define CHAT_WINDOW_STARTX 1
#define CHAT_WINDOW_STARTY 1
#define CHAT_WINDOW_CONTENT ""

#include <ncurses.h>

class Console {
  private:
    WINDOW *chat_window, *cmd_window;
    WINDOW* create_newwin(int height, int width, int starty, int startx);
    void destroy_win(WINDOW* local_win);
    int chat_curs_y;
    int cmd_curs_x;

  public:
    /**
     * Initialize and set up interactive console.
     */
    Console();
    /**
     * Terminate interactive console.
     */
    ~Console();
    /**
     * Whether interactive console is running or not.
     */
    bool running;
    /**
     * Process a user inputted command.
     */
    void process_command(char cmd[]);
    /**
     * Print a string to the chat window.
     */
    void print(std::string str);
    /**
     * Clear the chat window.
     */
    void clearchat();
    /**
     * Clear the cmd window.
     */
    void clearcmd();
    /**
     * Refresh the current frame.
     */
    void refresh();
    /**
     * Reset cursor to default position.
     */
    void reset_curs();
    /**
     * Read input from user.
     */
    std::string read();

    char getchar();
    /**
     * Exit console.
     */
    void exit();
};

#endif