#ifndef _SERVER_H_
#define _SERVER_H_

#define MESSAGE_SIZE 512

#define IP "IP"
#define PORT "PORT"
#define LIST "LIST"
#define LOGIN "LOGIN"
#define REFRESH "REFRESH"
#define SEND "SEND"
#define BROADCAST "BROADCAST"
#define BLOCK "BLOCK"
#define BLOCKED "BLOCKED"
#define UNBLOCK "UNBLOCK"
#define LOGOUT "LOGOUT"
#define EXIT "EXIT"
#define STATISTICS "STATISTICS"
#define AUTHOR "AUTHOR"

using std::string;
using std::vector;

class Server {
  private:
    struct Connection {
        int fd;
        string remote_ip;
        string fqdn;
        string port;
        bool active;
    };
    Console* console;
    vector<Connection> client_connections;
    void* get_in_addr(struct sockaddr* sa);
    /**
     * Initialize a new socket on specified port
     * @param  port The port to listen on
     * @return      A valid file descriptor > 0, negative otherwise
     */
    int init_socket(string port);
    /**
     * Handle a new connection from a client by initializing a new socket.
     * @param  listener The file descriptor corresponding to the socket
     * listening for new connections
     * @return          A valid file descriptor > 0, negative otherwise
     */
    int new_connection_handler(int listener);
    /**
     * Processing incoming data string from client.
     * @param sockfd The file descriptor the data was read from
     * @param data   The data that was read
     */
    void process_data(int sockfd, string data);
    int ip_to_fd(string ip);
    string fd_to_ip(int fd);
    /**
     * Send data to the client in the form of a string.
     * @param  str      The string to send
     * @param  clientfd The file descriptor corresponding to the appropriate
     * socket
     * @return          0 success, negative otherwise
     */
    int relay_to_client(string str, int clientfd, int senderfd);

    void send_client_list(int clientfd);
    int send_to_client(int clientfd, char buf[]);
    void broadcast_to_all(string msg, int senderfd);
    void add_connection(Connection c);
    /**
     * Process a user inputted command from STDIN.
     * @return 0 success, negative otherwise
     */
    int process_command();

  public:
    Server();
    ~Server();
    int launch(string port);
};

#endif