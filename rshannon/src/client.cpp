/*
* @Author: Robert Shannon <rshannon@buffalo.edu>
* @Date:   2016-02-05 21:41:26
* @Last Modified by:   Bobby
* @Last Modified time: 2016-02-08 22:47:20
*/

#include <vector>
#include <iterator>
#include <string>
#include <cstring>
#include <sstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#include "../include/console.h"
#include "../include/client.h"
#include "../include/error.h"

using std::string;
using std::istringstream;
using std::istream_iterator;
using std::vector;

Client::Client() {
    logged_in = false;
    sockfd = -1;
}

Client::~Client() {}

// get sockaddr, IPv4 or IPv6:
void* Client::get_in_addr(struct sockaddr* sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

void Client::process_command(string cmd) {
    string operation;
    istringstream buf(cmd);
    istream_iterator<string> beg(buf), end;
    vector<string> args(beg, end);

    // Grab the operation from the user inputted
    // command, i.e. LOGIN, EXIT, AUTHOR, etc.
    operation = args[0];

    if (logged_in) {
        // Commands allowed to be processed ONLY when
        // the client is logged in to a server.
        // Note that EXIT, and AUTHOR are allowed to be
        // processed regardless of whether the client
        // is logged into a server or not.
        if (operation == IP) {
            console->print("IP");
        } else if (operation == PORT) {
            console->print("PORT");
        } else if (operation == LIST) {
            console->print("LIST");
        } else if (operation == SEND) {
            console->print("SEND");
        } else if (operation == BROADCAST) {
            console->print("BROADCAST");
        } else if (operation == BLOCK) {
            console->print("BLOCK");
        } else if (operation == BLOCKED) {
            console->print("BLOCKED");
        } else if (operation == UNBLOCK) {
            console->print("UNBLOCK");
        } else if (operation == LOGOUT) {
            logout();
        } else if (operation == EXIT) {
            exit();
        } else if (operation == STATISTICS) {
            console->print("STATISTICS");
        } else if (operation == AUTHOR) {
            author();
        }
    } else {
        // Commands allowed to be processed when client is
        // NOT logged into a server.
        // Note that EXIT, and AUTHOR are allowed to be
        // processed regardless of whether the client
        // is logged into a server or not.
        if (operation == EXIT) {
            exit();
        } else if (operation == AUTHOR) {
            author();
        } else if (operation == LOGIN) {
            // LOGIN <HOST> <PORT>
            login(args[1], args[2]);
        } else {
            notify_error(
                operation,
                "You must be logged into a server to run this command.");
        }
    }
}

void Client::author() {
    notify_success(AUTHOR, "I, rshannon, have read and understood the course "
                           "academic integrity policy.");
}

void Client::ip() {}

void Client::port() {}

void Client::list() {}

int Client::server_connect(string host, string port) {
    int sockfd, nbytes, results;
    char s[INET6_ADDRSTRLEN];
    struct addrinfo hints, *servinfo;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((results =
             getaddrinfo(host.c_str(), port.c_str(), &hints, &servinfo)) != 0) {
        return ERR_CONNECTION;
    }

    // loop through all the results and connect to the first we can
    while (servinfo != NULL) {
        if ((sockfd = socket(servinfo->ai_family, servinfo->ai_socktype,
                             servinfo->ai_protocol)) == -1) {
            servinfo = servinfo->ai_next;
            continue;
        } else if (connect(sockfd, servinfo->ai_addr, servinfo->ai_addrlen) ==
                   -1) {
            close(sockfd);
            servinfo = servinfo->ai_next;
            continue;
        }

        break;
    }

    if (servinfo == NULL) {
        return ERR_CONNECTION;
    }

    inet_ntop(servinfo->ai_family,
              get_in_addr((struct sockaddr*)servinfo->ai_addr), s, sizeof s);

    freeaddrinfo(servinfo);

    return sockfd;
}

int Client::server_disconnect() {
    if (logged_in) {
        close(sockfd);
        return 0;
    }
    return ERR_NOT_LOGGED_IN;
}

void Client::login(string host, string port) {
    char data[MAXDATASIZE]; // Data received buffer
    int nbytes;
    string result;
    sockfd = server_connect(host, port);

    if (is_err(sockfd)) {
        notify_error(LOGIN, err_to_str(sockfd));
        return;
    }

    if ((nbytes = recv(sockfd, data, MAXDATASIZE - 1, 0)) == -1) {
        // errmsg = string(strerror(nbytes));
        notify_error(LOGIN, err_to_str(ERR_CONNECTION));
        return;
    }

    result = "client: received " + string(data);
    notify_success(LOGIN, result);
    logged_in = true;
}

void Client::refresh() {}

void Client::broadcast() {}

void Client::block() {}

void Client::unblock() {}

void Client::logout() {
    int ret = server_disconnect();
    if (is_err(ret)) {
        notify_error(LOGOUT, err_to_str(ret));
    } else {
        logged_in = false;
        notify_success(LOGOUT, "Successfully logged out from server.");
    }
}

void Client::notify_success(string operation, string results) {
    console->print("[" + operation + ":SUCCESS]\n");
    console->print(results + "\n");
    console->print("[" + operation + ":END]\n");
}

void Client::notify_error(string operation, string error) {
    console->print("[" + operation + ":ERROR]\n");
    console->print(error + "\n");
    console->print("[" + operation + ":END]\n");
}

void Client::exit() {
    if (logged_in) {
        logout();
    }
    notify_success(EXIT, "Terminating...");
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