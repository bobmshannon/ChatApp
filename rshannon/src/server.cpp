/*
* @Author: Robert Shannon <rshannon@buffalo.edu>
* @Date:   2016-02-05 21:26:31
* @Last Modified by:   Bobby
* @Last Modified time: 2016-02-11 20:59:01
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

using std::string;
using std::strcpy;
using std::istringstream;
using std::istream_iterator;
using std::vector;

void* Server::get_in_addr(struct sockaddr* sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

Server::Server() { client_connections = vector<Connection>(); }

Server::~Server() {}

void Server::process_data(int sockfd, string data) {
    string operation, msg;
    int clientfd;
    istringstream buf(data);
    istream_iterator<string> beg(buf), end;
    vector<string> args(beg, end);

    // Grab the operation from the user inputted
    // command, i.e. LOGIN, EXIT, AUTHOR, etc.
    operation = args[0];

    if (operation == SEND) {
    	for(int i = 2; i < args.size(); i++) {
    		msg += (args[i] + " ");
    	}
    	if((clientfd = ip_to_fd(args[1])) != -1) {
    		relay_to_client(msg, clientfd, sockfd);
    	}
    } else if (operation == BROADCAST) {
    	for(int i = 1; i < args.size(); i++) {
    		msg += (args[i] + " ");
    	}
		broadcast_to_all(msg, sockfd);
    }
}

int Server::ip_to_fd(string ip) {
	for(int i = 0; i < client_connections.size(); i++) {
		if(client_connections[i].remote_ip == ip) {
			return client_connections[i].fd;
		}
	}
	return -1;
}

string Server::fd_to_ip(int fd) {
	for(int i = 0; i < client_connections.size(); i++) {
		if(client_connections[i].fd == fd) {
			return client_connections[i].remote_ip;
		}
	}
	return "";
}


int Server::process_command() {
    char buf[MESSAGE_SIZE];
    if (fgets(buf, MESSAGE_SIZE, stdin) == NULL) {
        return -1;
    }
    printf("You entered a command: %s", buf);
    return 0;
}

int Server::init_socket(string port) {
    int listener;
    int yes = 1; // for setsockopt() SO_REUSEADDR, below
    struct addrinfo hints, *ai, *p;

    // Get us a socket and bind it
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if (getaddrinfo(NULL, port.c_str(), &hints, &ai) != 0) {
        return ERR_SOCKET_INIT;
    }

    for (p = ai; p != NULL; p = p->ai_next) {
        listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (listener < 0) {
            continue;
        }

        setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

        if (bind(listener, p->ai_addr, p->ai_addrlen) < 0) {
            close(listener);
            continue;
        }

        break;
    }

    if (p == NULL) {
        return ERR_SOCKET_BIND;
    }

    freeaddrinfo(ai);

    // Listen for new connections on socket
    if (listen(listener, 10) == -1) {
        return ERR_SOCKET_LISTEN;
    }

    return listener;
}

int Server::relay_to_client(string str, int clientfd, int senderfd) {
	printf("relaying to client %i: %s", clientfd, str.c_str());
    char buf[MESSAGE_SIZE];
    string sender_ip, msg;

    if((sender_ip = fd_to_ip(senderfd)) != "") {
    	msg = "msg from:" + sender_ip + "\n[msg]:" + str;
    } else {
    	return -1;
    }

    for (int i = 0; i < msg.length(); i++) {
        buf[i] = msg[i];
        if (i == msg.length() - 1) {
            buf[i + 1] = '\n';
        }
    }

    return send_to_client(clientfd, buf);
}

void Server::send_client_list(int clientfd) {
	string client_list;
	char buf[MESSAGE_SIZE] = {'\0'};
	for(int i = 0; i < client_connections.size(); i++) {
		if(client_connections[i].active) {
			client_list += (client_connections[i].remote_ip + " ");
		}
	}
	strcpy(buf, client_list.c_str());
	send_to_client(clientfd, buf);
}

void Server::broadcast_to_all(string msg, int senderfd) {
	char buf[MESSAGE_SIZE];
	string sender_ip = fd_to_ip(senderfd);
	msg = "msg from:" + sender_ip + "\n[msg]:" + msg;
	strcpy(buf, msg.c_str());
	for(int i = 0; i < client_connections.size(); i++) {
		if(client_connections[i].fd != senderfd) {
			if(client_connections[i].active) {
				send_to_client(client_connections[i].fd, buf);
			} else {
				// TODO: add message to buffer
			}
		}
	}
}

int Server::send_to_client(int clientfd, char buf[]) {
    int total = 0;
    int bytesleft = MESSAGE_SIZE;
    int n;

    while (total < MESSAGE_SIZE) {
        n = send(clientfd, buf + total, bytesleft, 0);
        if (n == -1) {
            break;
        }
        total += n;
        bytesleft -= n;
    }

    return n == -1 ? -1 : 0;
}

int Server::new_connection_handler(int listener) {
    struct sockaddr_storage remoteaddr; // Client's IP address
    socklen_t addrlen;
    int newfd;

    // Handle new connection
    addrlen = sizeof remoteaddr;
    newfd = accept(listener, (struct sockaddr*)&remoteaddr, &addrlen);
    char remoteIP[INET6_ADDRSTRLEN];

    if (newfd == -1) {
        return ERR_SOCKET_ACCEPT;
    }
    printf("selectserver: new connection from %s on "
           "socket %d\n",
           inet_ntop(remoteaddr.ss_family,
                     get_in_addr((struct sockaddr*)&remoteaddr), remoteIP,
                     INET6_ADDRSTRLEN),
           newfd);

    // Keep track of new connection
    Connection connection = {newfd, string(remoteIP), true};

    client_connections.push_back(connection);

    // Send client list as welcome message
    send_client_list(newfd);

    return newfd;
}



int Server::launch(string port) {
    fd_set master, read_fds;
    int fdmax, listener, clientfd, nbytes;
    char buf[MESSAGE_SIZE] = {'\0'};

    // Clear the master and temp sets
    FD_ZERO(&master);
    FD_ZERO(&read_fds);

    // Initialize socket that listens for new connections
    if ((listener = init_socket(port)) < 0) {
        return listener;
    }

    // Add the listener and STDIN file descriptors to the master set
    FD_SET(listener, &master);
    FD_SET(0, &master);

    // Keep track of the biggest file descriptor
    fdmax = listener;

    // Main loop
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
                        // Unable to handle new connection
                    } else {
                        FD_SET(clientfd, &master); // add to master set
                        if (clientfd > fdmax) {    // keep track of the max
                            fdmax = clientfd;
                        }
                    }
                } else if (i == 0) {
                    // Input received from STDIN
                    process_command();

                } else {
                    // Data received from existing connection
                    if ((nbytes = recv(i, buf, MESSAGE_SIZE, 0)) <= 0) {
                        if (nbytes == 0) {
                            // Connection closed by client
                        } else {
                            // read() error
                        }
                        close(i);
                        FD_CLR(i, &master);
                    } else {
                        printf("received %i bytes from fd %i: %s", nbytes, i,
                               buf);
                        process_data(i, string(buf));
                    }
                }
            }
        }
    }

    return 0;
}
