/*
* @Author: Robert Shannon <rshannon@buffalo.edu>
* @Date:   2016-02-05 21:41:26
* @Last Modified by:   Bobby
* @Last Modified time: 2016-02-07 16:53:18
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

using std::string;
using std::istringstream;
using std::istream_iterator;
using std::vector;

Client::Client() {}

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

    if (operation == "IP") {

    } else if (operation == "PORT") {

    } else if (operation == "LIST") {

    } else if (operation == "LOGIN") {
        // LOGIN <HOST> <PORT>
        login(args[1], args[2]);
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

int Client::server_connect(string host, string port) {
    int sockfd, nbytes, results;
    char s[INET6_ADDRSTRLEN];
    struct addrinfo hints, *servinfo;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((results =
             getaddrinfo(host.c_str(), port.c_str(), &hints, &servinfo)) != 0) {
        console->print("getaddrinfo: " + string(gai_strerror(results)));
        // return 1;
    }

    // loop through all the results and connect to the first we can
    while (servinfo != NULL) {
        if ((sockfd = socket(servinfo->ai_family, servinfo->ai_socktype,
                             servinfo->ai_protocol)) == -1) {
            perror("client: socket");
            servinfo = servinfo->ai_next;
            continue;
        } else if (connect(sockfd, servinfo->ai_addr, servinfo->ai_addrlen) ==
                   -1) {
            close(sockfd);
            perror("client: connect");
            servinfo = servinfo->ai_next;
            continue;
        }

        break;
    }

    if (servinfo == NULL) {
        console->print("client: failed to connect");
        // return 2;
    }

    inet_ntop(servinfo->ai_family,
              get_in_addr((struct sockaddr*)servinfo->ai_addr), s, sizeof s);
    console->print("client: connecting to " + string(s));

    freeaddrinfo(servinfo); // all done with this structure

    return sockfd;
}

int Client::server_disconnect(int sockfd) {
	close(sockfd);
	return 0;
}

void Client::login(string host, string port) {
	char data[MAXDATASIZE];
	int sockfd, nbytes;

	sockfd = server_connect(host, port);

    if ((nbytes = recv(sockfd, data, MAXDATASIZE - 1, 0)) == -1) {
        perror("recv");
        // exit(1);
    }

    data[nbytes] = '\0';

    console->print("client: received " + string(data));

    server_disconnect(sockfd);
}

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
    // todo: logout from remote server
    notify_success("EXIT", "Terminating...");
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