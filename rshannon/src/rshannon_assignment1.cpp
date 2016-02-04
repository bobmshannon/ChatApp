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
#include <string.h>
#include <curses.h>
#include <stdlib.h>

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

    // Argument checking
    if (argc != 3) {
        cerr << "Usage: " + string(argv[0]) + " <MODE> <PORT>" << endl
             << "    <MODE> [s/c]" << endl
             << "    Indicates whether to run program in server or client mode"
             << endl
             << "    <PORT> [0-65535]" << endl
             << "    The port number to listen on." << endl;
        // return -1;
    } else if (string(argv[1]) != "s" && string(argv[1]) != "c") {
        // return -1;
    } else if (atoi(argv[2]) > 65535 || atoi(argv[2]) < 0) {
        // return -1;
    }

    // Fetch user input
    Console* console = new Console();
    string cmd;

    while (console->running) {
        cmd = console->read();
        console->print(cmd);
    }

    delete console;
}