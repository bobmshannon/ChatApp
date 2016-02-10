/*
* @Author: Robert Shannon <rshannon@buffalo.edu>
* @Date:   2016-02-05 21:26:31
* @Last Modified by:   Bobby
* @Last Modified time: 2016-02-10 17:28:35
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
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>

#include "../include/console.h"
#include "../include/server.h"
#include "../include/error.h"

// get sockaddr, IPv4 or IPv6:
void* Server::get_in_addr(struct sockaddr* sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

Server::Server() {
	active_connections = std::vector<Connection>();
}

Server::~Server() {}

void Server::process_data(int sockfd) {

}

void Server::process_command() {}

int Server::init_socket(std::string port) {
    int listener, rv;
    int yes = 1; // for setsockopt() SO_REUSEADDR, below
    struct addrinfo hints, *ai, *p;

    // get us a socket and bind it
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    if ((rv = getaddrinfo(NULL, "2512", &hints, &ai)) != 0) {
        return ERR_SOCKET_INIT;
    }

    for (p = ai; p != NULL; p = p->ai_next) {
        listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (listener < 0) {
            continue;
        }

        // lose the pesky "address already in use" error message
        setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

        if (bind(listener, p->ai_addr, p->ai_addrlen) < 0) {
            close(listener);
            continue;
        }

        break;
    }

    // if we got here, it means we didn't get bound
    if (p == NULL) {
        return ERR_SOCKET_BIND;
    }

    freeaddrinfo(ai); // all done with this

    // Listen for new connections on socket
    if (listen(listener, 10) == -1) {
        return ERR_SOCKET_LISTEN;
    }

    return listener;
}

int Server::new_connection_handler(int listener) {
    struct sockaddr_storage remoteaddr; // client address
    socklen_t addrlen;
    int newfd;

    // Handle new connection
    addrlen = sizeof remoteaddr;
    newfd = accept(listener, (struct sockaddr*)&remoteaddr, &addrlen);
    char remoteIP[INET6_ADDRSTRLEN];

    if (newfd == -1) {
        return ERR_SOCKET_ACCEPT;
    } else {
        printf("selectserver: new connection from %s on "
               "socket %d\n",
               inet_ntop(remoteaddr.ss_family,
                         get_in_addr((struct sockaddr*)&remoteaddr), remoteIP,
                         INET6_ADDRSTRLEN),
               newfd);
        send(newfd, "Connection ACK.\0", 16, 0);
    }

    // Keep track of new connection
    Connection connection = {
    	newfd,
    	std::string(remoteIP)
    };

    active_connections.push_back(connection);

    return newfd;
}

int Server::launch() {
    fd_set master, read_fds;
    int fdmax, listener, clientfd, nbytes;
    char buf[BUFFER_SIZE];

    // Clear the master and temp sets
    FD_ZERO(&master);
    FD_ZERO(&read_fds);

    // Initialize socket that listens for new connections
    if ((listener = init_socket("2512")) < 0) {
        return listener;
    }

    // Add the listener and STDIN file descriptors to the master set
    FD_SET(listener, &master);
    FD_SET(0, &master);

    // Keep track of the biggest file descriptor
    fdmax = listener;

    // main loop
    while (1) {
        read_fds = master;

        if (select(fdmax + 1, &read_fds, NULL, NULL, NULL) == -1) {
            perror("select");
            exit(4);
        }

        for (int i = 0; i <= fdmax; i++) {
            // Check whether file descriptor is ready to be read
            if (FD_ISSET(i, &read_fds)) {
                if (i == listener) {
                    // New connection received
                    if ((clientfd = new_connection_handler(listener)) == -1) {
                        return clientfd;
                    } else {
                        FD_SET(clientfd, &master); // add to master set
                        if (clientfd > fdmax) {    // keep track of the max
                            fdmax = clientfd;
                        }
                    }
                } else if (i == 0) {
                    // Input received from STDIN
                    process_command();

                    /*if (fgets(buf, BUFFER_SIZE, stdin)) {
                        printf("A key was pressed.\n");
                    }*/
                } else {
                    // Data received from existing connection
                    if ((nbytes = recv(i, buf, sizeof buf, 0)) <= 0) {
                        if (nbytes == 0) {
                            // Connection closed by client
                            return ERR_CONN_CLOSED;
                        } else {
                            // read() error
                            return ERR_SOCKET_READ;
                        }
                        close(i);
                        FD_CLR(i, &master);
                    } else {
                        // Process data received
                        for (int j = 0; j <= fdmax; j++) {
                            if (FD_ISSET(j, &master)) {
                            	// Process incoming data
                            	process_data(j);
                            }
                        }
                    }
                }
            }
        }
    }

    return 0;

    /*
// Fetch user input
string cmd;
console = new Console();

while (console->running) {
    cmd = console->read();

    // Process user inputted command
    process_command(cmd);
}*/
}
