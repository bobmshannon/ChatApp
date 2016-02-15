/*
* @Author: Robert Shannon <rshannon@buffalo.edu>
* @Date:   2016-02-05 21:41:26
* @Last Modified by:   Bobby
* @Last Modified time: 2016-02-15 00:30:18
*/

#include <vector>
#include <iterator>
#include <string>
#include <iostream>
#include <cstring>
#include <sstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#include "../include/client.h"
#include "../include/error.h"
#include "../include/logger.h"

using std::string;
using std::istringstream;
using std::istream_iterator;
using std::vector;

Client::Client() {
    logged_in = false;
    sockfd = -1;
}

Client::~Client() {}

void* Client::get_in_addr(struct sockaddr* sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

void Client::process_command(string cmd) {
    string operation, msg;
    istringstream buf(cmd);
    istream_iterator<string> beg(buf), end;
    vector<string> args(beg, end);

    // Grab the operation from the user inputted
    // command, i.e. LOGIN, EXIT, AUTHOR, etc.
    if (args.size() > 0) {
        operation = args[0];
    } else {
        return;
    }

    if (logged_in) {
        // Commands allowed to be processed ONLY when
        // the client is logged in to a server.
        // Note that EXIT, and AUTHOR are allowed to be
        // processed regardless of whether the client
        // is logged into a server or not.
        if (operation == IP) {
            cse4589_print_and_log("%s", operation.c_str());
        } else if (operation == PORT) {
            port();
        } else if (operation == LIST) {
            // LIST
            list();
        } else if (operation == SEND) {
            for (int i = 2; i < args.size(); i++) {
                msg += (args[i] + " ");
            }
            // SEND <IP> <MSG>
            send_msg(args[1], msg);
        } else if (operation == BROADCAST) {
            for (int i = 1; i < args.size(); i++) {
                msg += (args[i] + " ");
            }
            // BROADCAST <MSG>
            broadcast(msg);
        } else if (operation == BLOCK) {
            if (args.size() != 2) {
                notify_error(BLOCK, "Usage: BLOCK <client-ip>");
            } else {
                // BLOCK <CLIENT-IP>
                block_client(args[1]);
            }
        } else if (operation == UNBLOCK) {
            if (args.size() != 2) {
                notify_error(UNBLOCK, "Usage: UNBLOCK <client-ip>");
            } else {
                // UNBLOCK <CLIENT-IP>
                unblock_client(args[1]);
            }
        } else if (operation == LOGOUT) {
            logout();
        } else if (operation == EXIT) {
            exit_client();
        } else if (operation == AUTHOR) {
            author();
        } else if (operation == REFRESH) {
            refresh();
        } else if (operation == LOGIN) {
            notify_error(operation, "You are already logged in to a server.");
        } else {
            notify_error(operation, "You entered an invalid command.");
        }
    } else {
        // Commands allowed to be processed when client is
        // NOT logged into a server.
        // Note that EXIT, and AUTHOR are allowed to be
        // processed regardless of whether the client
        // is logged into a server or not.
        if (operation == IP || operation == LIST ||
            operation == SEND || operation == BROADCAST || operation == BLOCK ||
            operation == BLOCKED || operation == UNBLOCK ||
            operation == LOGOUT || operation == STATISTICS) {
            notify_error(
                operation,
                "You must be logged into a server to run this command.");
        } else if (operation == EXIT) {
            exit_client();
        } else if (operation == AUTHOR) {
            author();
        } else if (operation == PORT) {
            port();
        } else if (operation == LOGIN) {
            // LOGIN <HOST> <PORT>
            if (args.size() == 3) {
                login(args[1], args[2]);
            } else {
                notify_error(LOGIN, "Usage: LOGIN <HOST> <PORT>");
            }
        } else {
            notify_error(operation, "You entered an invalid command.");
        }
    }
}

int Client::is_valid_ip(string ip) {
    struct sockaddr_in sa;

    // Check if valid IP address.
    if (inet_pton(AF_INET, ip.c_str(), &sa.sin_addr) <= 0) {
        return -1;
    } else {
        return 0;
    }
}

void Client::send_msg(string ip, string msg) {
    if (is_valid_ip(ip) == -1) {
        notify_error(SEND,
                     "That IP address seems to not be a valid IPv4 address.");
    } else {
        // TODO: check the IP in client_list, and whether it is valid
        send_to_server(string(SEND) + " " + ip + " " + msg);
        notify_success(SEND, "Message sent.");
    }
}

int Client::send_to_server(string str) {
    char buf[MESSAGE_SIZE] = {'\0'};
    for (int i = 0; i < str.length(); i++) {
        buf[i] = str[i];
        if (i == str.length() - 1) {
            buf[i + 1] = '\n';
        }
    }
    int total = 0;
    int bytesleft = MESSAGE_SIZE;
    int n;

    while (total < MESSAGE_SIZE) {
        n = send(sockfd, buf + total, bytesleft, 0);
        if (n == -1) {
            break;
        }
        total += n;
        bytesleft -= n;
    }

    return n == -1 ? -1 : 0;
}

void Client::author() {
    notify_success(AUTHOR, "I, rshannon, have read and understood the course "
                           "academic integrity policy.");
}

void Client::block_client(string ip) {
    if (is_valid_ip(ip) == -1) {
        notify_error(BLOCK, "That is not a valid IPv4 address");
    } else if (send_to_server(string(BLOCK) + " " + ip) == 0) {
        notify_success(BLOCK, ip + " has been blocked.");
    } else {
        notify_error(BLOCK, "");
    }
}

void Client::unblock_client(string ip) {
    if (is_valid_ip(ip) == -1) {
        notify_error(UNBLOCK, "That is not a valid IPv4 address");
    } else if (send_to_server(string(UNBLOCK) + " " + ip) == 0) {
        notify_success(UNBLOCK, ip + " has been unblocked.");
    } else {
        notify_error(UNBLOCK, "");
    }
}

void Client::ip() {}

void Client::port() {
    notify_success(PORT, "PORT:" + listen_port);
    /*
    char buf[MESSAGE_SIZE];
    string port;
    if (send_to_server(PORT) != -1) {
        if (recv(sockfd, buf, MESSAGE_SIZE, 0) <= 0) {
            notify_error(PORT, "Unable to get port from server.");
            return;
        }
        port = string(buf);
    }
    notify_success(PORT, "PORT:" + port);*/
}

void Client::refresh() {
    char buf[MESSAGE_SIZE];
    if (send_to_server(LIST) != -1) {
        if (recv(sockfd, buf, MESSAGE_SIZE, 0) <= 0) {
            notify_error(REFRESH,
                         "Unable to get updated client list from server.");
            return;
        }
        client_list = string(buf);
    }
    notify_success(REFRESH, client_list);
}

void Client::list() { notify_success(LIST, client_list); }

int Client::server_connect(string host, string port) {
    int sockfd;
    char s[INET6_ADDRSTRLEN];
    struct addrinfo hints, *servinfo;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((getaddrinfo(host.c_str(), port.c_str(), &hints, &servinfo)) != 0) {
        return ERR_CONNECTION;
    }

    // Loop through all the results and connect to the first we can
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
    char data[MESSAGE_SIZE];
    string result;
    sockfd = server_connect(host, port);

    if (is_err(sockfd)) {
        notify_error(LOGIN, err_to_str(sockfd));
        return;
    }

    if (recv(sockfd, data, MESSAGE_SIZE, 0) == -1) {
        notify_error(LOGIN, err_to_str(ERR_CONNECTION));
        return;
    }

    result = string(data);
    client_list = result;
    notify_success(LOGIN, result);
    logged_in = true;
}

void Client::broadcast(string msg) {
    send_to_server(string(BROADCAST) + " " + msg);
    notify_success(BROADCAST, "Broadcast message sent.");
}

void Client::logout() {
    send_to_server(LOGOUT);
    int ret = server_disconnect();
    if (is_err(ret)) {
        notify_error(LOGOUT, err_to_str(ret));
    } else {
        logged_in = false;
        send_to_server(string(LOGOUT));
        notify_success(LOGOUT, "Successfully logged out from server.");
    }
}

void Client::notify_success(string operation, string results) {
    cse4589_print_and_log("[%s:SUCCESS]\n", operation.c_str());
    cse4589_print_and_log("%s\n", results.c_str());
    cse4589_print_and_log("[%s:END]\n", operation.c_str());
}

void Client::notify_error(string operation, string error) {
    cse4589_print_and_log("[%s:ERROR]\n", operation.c_str());
    cse4589_print_and_log("%s\n", error.c_str());
    cse4589_print_and_log("[%s:END]\n", operation.c_str());
}

void Client::exit_client() {
    if (logged_in) {
        logout();
    }
    notify_success(EXIT, "Terminating...");
    exit(0);
}

void Client::prompt_login() {
    // Fetch user input
    string cmd;

    while (!logged_in) {
        getline(std::cin, cmd);

        // Process user inputted command
        process_command(cmd);
    }

    // Clear the master and temp sets
    FD_ZERO(&master);
    FD_ZERO(&read_fds);

    // Add the listener and STDIN file descriptors to the master set
    FD_SET(sockfd, &master);
    FD_SET(0, &master);
}

void Client::launch(string port) {
    listen_port = port;
    
    prompt_login();

    string cmd = "";

    int fdmax, nbytes;
    char buf[MESSAGE_SIZE + 1] = {'\0'};

    // Keep track of the biggest file descriptor
    fdmax = sockfd;

    // Main loop
    while (1) {
        read_fds = master;

        if (select(fdmax + 1, &read_fds, NULL, NULL, NULL) == -1) {
            perror("select");
        }

        for (int i = 0; i <= fdmax; i++) {
            // Check whether file descriptor is ready to be read
            if (FD_ISSET(i, &read_fds)) {
                if (i == sockfd) {
                    // Message received from server
                    if ((nbytes = recv(i, buf, MESSAGE_SIZE, 0)) <= 0) {
                        if (nbytes == 0) {
                            // Connection closed by client
                        } else {
                            // read() error
                        }
                        close(i);
                        FD_CLR(i, &master);
                        exit(-1);
                    } else {
                        // Print welcome message from server
                        cse4589_print_and_log("%s", buf);
                    }
                } else if (i == 0) {
                    // Input received from STDIN
                    getline(std::cin, cmd);
                    process_command(cmd);
                    if (!logged_in) {
                        prompt_login();
                    }
                }
            }
        }
    }
}