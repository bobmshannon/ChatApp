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
#include <string>
#include <stdio.h>
#include <curses.h>
#include <stdlib.h>
#include <vector>

#include "../include/global.h"
#include "../include/logger.h"
#include "../include/console.h"
#include "../include/commands.h"
#include "../include/client.h"

using namespace std;

void show_usage(string progname) {
    cerr << "Usage: " + progname + " <MODE> <PORT>" << endl
         << "    <MODE> [s/c]" << endl
         << "    Indicates whether to run program in server or client mode"
         << endl
         << "    <PORT> [0-65535]" << endl
         << "    The port number to listen on." << endl;
}

/**
* main function
* @param  argc Number of arguments
* @param  argv The argument list
* @return 0 EXIT_SUCCESS
*/
int main(int argc, char** argv) {
    /*Init. Logger*/
    cse4589_init_log(argv[2]);

    /* Clear LOGFILE*/
    fclose(fopen(LOGFILE, "w"));

    // Argument checking
    if (argc != 3) {
        show_usage(string(argv[0]));
        return -1;
    }

    // Get arguments
    string progname(argv[0]);
    string mode(argv[1]);
    string port(argv[2]);

    // More argument checking
    if (mode != "s" && mode != "c") {
        show_usage(string(argv[0]));
        return -1;
    } else if (stoi(port) > 65535 || stoi(port) < 0) {
        show_usage(string(argv[0]));
        return -1;
    }

    // Initialize Logger
    cse4589_init_log(argv[2]);

    // Clear LOGFILE
    FILE* fp = fopen(LOGFILE, "w");
    if (fp != NULL) {
        fclose(fp);
    }

    if (mode == "s") {

    } else if (mode == "c") {
        Client client = Client();
        client.launch();
    }
}