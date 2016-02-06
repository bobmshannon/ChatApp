/*
* @Author: Robert Shannon <rshannon@buffalo.edu>
* @Date:   2016-02-05 21:41:26
* @Last Modified by:   Bobby
* @Last Modified time: 2016-02-05 23:46:19
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

	if(operation == "IP") {

	} 
	else if (operation == "PORT") {

	}
	else if (operation == "LIST") {

	}
	else if (operation == "LOGIN") {
		
	}
	else if (operation == "REFRESH") {
		
	}
	else if (operation == "SEND") {
		
	}
	else if (operation == "BROADCAST") {
		
	}
	else if (operation == "BLOCK") {
		
	}
	else if (operation == "BLOCKED") {
		
	}
	else if (operation == "UNBLOCK") {
		
	}
	else if (operation == "LOGOUT") {
		
	}
	else if (operation == "EXIT") {
		
	}
	else if (operation == "STATISTICS") {
		
	}

	console->print(operation);
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

void Client::exit() {
    console->exit();
    delete console;
}