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
    struct Message {
        string sender_ip;
        string receiver_ip;
        string msg;
    };
    struct Connection {
        int fd;           // The file descriptor of the socket
        int num_sent;     // Number messages sent by this client
        int num_recv;     // Number of messages received by this client
        string remote_ip; // The external IP address of this client
        string fqdn;      // The hostname of this client
        string port;      // The local port of this client
        bool active;      // Whether this client is currently logged in or not
        vector<string>
            blocked; // List of client IP addresses to block messages from
        vector<Message> msg_buffer; // List of buffered messages
    };
    Console* console;
    vector<Connection> client_connections;
    string listen_port;
    void* get_in_addr(struct sockaddr* sa);
    static bool compare_by_port(const Connection& a, const Connection& b);
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
    int send_buffered_messages(int fd);
    int buffer_message(string senderip, string receiverip, string msg);
    void send_client_list(int clientfd);
    string get_client_list();
    int send_to_client(int clientfd, char buf[]);
    void broadcast_to_all(string msg, int senderfd);
    void add_connection(Connection c);
    int get_connection(int fd);
    int increment_num_sent(int fd);
    int increment_num_recv(int fd);
    void block(int clientfd, string blockedip);
    void unblock(int clientfd, string blockedip);
    void exit_server();
    void blocked(string clientip);
    void statistics();
    /**
     * Updates the listen port of the specified client
     * @param fd   The fd associated with the client
     * @param port The port that the client is listening on
     */
    void port(int fd, string port);
    void author();
    int logout(int fd);
    void notify_success(string operation, string results);
    void notify_error(string operation, string error);
    bool is_known_ip(string ip);
    bool is_online(string ip);
    /**
     * Check whether client associated with specified fd has blocked the
     * IP address.
     * @param  fd The fd of the client
     * @param  ip The ip to check if whether is blocked by client or not
     * @return    true if blocked, false otherwise
     */
    bool is_blocked(int fd, string ip);
    bool is_valid_ip(string ip);
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