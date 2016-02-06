/*
* @Author: Robert Shannon <rshannon@buffalo.edu>
* @Date:   2016-02-05 21:41:26
* @Last Modified by:   Bobby
* @Last Modified time: 2016-02-06 18:55:38
*/

#include <string>
#include <sstream>

#include "../include/console.h"
#include "../include/client.h"

using std::string;
using std::stringstream;

Client::Client() {}

Client::~Client() {}

void Client::process_command(string cmd) {
    // Grab the operation from the user inputted
    // command, i.e. LOGIN, EXIT, AUTHOR, etc.
    stringstream stream(cmd);
    string operation;

    stream >> operation;

    if (operation == "IP") {

    } else if (operation == "PORT") {

    } else if (operation == "LIST") {

    } else if (operation == "LOGIN") {

    } else if (operation == "REFRESH") {

    } else if (operation == "SEND") {

    } else if (operation == "BROADCAST") {

    } else if (operation == "BLOCK") {

    } else if (operation == "BLOCKED") {

    } else if (operation == "UNBLOCK") {

    } else if (operation == "LOGOUT") {

    } else if (operation == "EXIT") {
    	exit();
    } else if (operation == "STATISTICS") {

    } else if (operation == "AUTHOR") {
        author();
    }

    // console->print(operation);
}

void Client::author() {
    notify_success("AUTHOR", "I, rshannon, have read and understood the course "
                             "academic integrity policy.");
}

void Client::ip() {}

void Client::port() {}

void Client::list() {}

void Client::login() {}

void Client::refresh() {}

void Client::broadcast() {}

void Client::block() {}

void Client::unblock() {}

void Client::logout() {}

void Client::notify_success(string operation, string results) {
    console->print("[" + operation + ":SUCCESS]\n");
    console->print(results + "\n");
    console->print("[" + operation + ":END]\n");
}

void Client::exit() {
	//todo: logout from remote server
    console->exit();
    delete console;
}

void Client::launch() {
    // Fetch user input
    string cmd;
    console = new Console();

    while (console->running) {
        cmd = console->read();

        // Process user inputted command
        process_command(cmd);
    }
}