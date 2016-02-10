/*
* @Author: Robert Shannon <rshannon@buffalo.edu>
* @Date:   2016-02-05 21:26:31
* @Last Modified by:   Bobby
* @Last Modified time: 2016-02-10 15:48:22
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

void Server::sigchld_handler(int s) {
    // waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;

    while (waitpid(-1, NULL, WNOHANG) > 0)
        ;

    errno = saved_errno;
}

// get sockaddr, IPv4 or IPv6:
void* Server::get_in_addr(struct sockaddr* sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

Server::Server() {}

Server::~Server() {}

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
        fprintf(stderr, "selectserver: %s\n", gai_strerror(rv));
        exit(1);
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
        // fprintf(stderr, "selectserver: failed to bind\n");
        return -1;
    }

    freeaddrinfo(ai); // all done with this

    // Listen for new connections on socket
    if (listen(listener, 10) == -1) {
    	return -1;
        //perror("listen");
    }

    return listener;
}

int Server::new_connection_handler(int listener) {
    struct sockaddr_storage remoteaddr; // client address
    socklen_t addrlen;
    int newfd;

    // handle new connections
    addrlen = sizeof remoteaddr;
    newfd = accept(listener, (struct sockaddr*)&remoteaddr,
                   &addrlen);
    char remoteIP[INET6_ADDRSTRLEN];

    if (newfd == -1) {
    	return -1;
    } else {
        printf("selectserver: new connection from %s on "
               "socket %d\n",
               inet_ntop(
                   remoteaddr.ss_family,
                   get_in_addr((struct sockaddr*)&remoteaddr),
                   remoteIP, INET6_ADDRSTRLEN),
               newfd);
        send(newfd, "Connection ACK.\0", 16, 0);
    }

    return newfd;
}

int Server::launch() {
    fd_set master, read_fds;
    int fdmax, listener, clientfd, nbytes;
    char buf[256];

    // Clear the master and temp sets
    FD_ZERO(&master); 
    FD_ZERO(&read_fds);

    // Initialize socket that listens for new connections
    if((listener = init_socket("2512")) == -1) {
    	return -1;
    }

    // Add the listener and STDIN file descriptors to the master set
    FD_SET(listener, &master);
    FD_SET(0, &master);

    // Keep track of the biggest file descriptor
    fdmax = listener;

    // main loop
    while(1) {
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
                	if((clientfd = new_connection_handler(listener)) == -1) {
                		return -1;
					} else {
				        FD_SET(clientfd, &master); // add to master set
				        if (clientfd > fdmax) {    // keep track of the max
				            fdmax = clientfd;
				        }
					}
                } else if (i == 0) {
                	// Input received from STDIN
                	if(fgets(buf,256, stdin)) {
                		printf("A key was pressed.\n");
                	} else {
                		printf("fail");
                	}
                } else {
                    // Data received from existing connection
                    if ((nbytes = recv(i, buf, sizeof buf, 0)) <= 0) {
                        // got error or connection closed by client
                        if (nbytes == 0) {
                            // connection closed
                            printf("selectserver: socket %d hung up\n", i);
                        } else {
                            perror("recv");
                        }
                        close(i);           // bye!
                        FD_CLR(i, &master); // remove from master set
                    } else {
                        // we got some data from a client
                        for (int j = 0; j <= fdmax; j++) {
                            // send to everyone!
                            if (FD_ISSET(j, &master)) {
                                // except the listener and ourselves
                                if (j != listener && j != i) {
                                    if (send(j, buf, nbytes, 0) == -1) {
                                        perror("send");
                                    }
                                }
                            }
                        }
                    }
                } // END handle data from client
            }     // END got new incoming connection
        }         // END looping through file descriptors
    }             // END for(;;)--and you thought it would never end!

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
