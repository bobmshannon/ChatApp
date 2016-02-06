/*
* @Author: Robert Shannon <rshannon@buffalo.edu>
* @Date:   2016-02-05 21:41:26
* @Last Modified by:   Bobby
* @Last Modified time: 2016-02-05 23:25:06
*/

#include <string>

#include "../include/console.h"
#include "../include/client.h"

using std::string;

Client::Client() {}

Client::~Client() {}

void Client::launch() {
    // Fetch user input
    string cmd;
    console = new Console();

    while (console->running) {
        cmd = console->read();
        console->print(cmd);
    }
}

void Client::exit() {
    console->exit();
    delete console;
}