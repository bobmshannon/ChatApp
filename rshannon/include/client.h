#ifndef _CLIENT_H_
#define _CLIENT_H_

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

class Client {
  private:
    int sockfd;         // File descriptor for network socket
    bool logged_in;     // Whether client is logged into a remote server
    string client_list; // Most recent list of clients retrieved from server
    fd_set master, read_fds;
    void* get_in_addr(struct sockaddr* sa);
    /**
     * Initialize a socket for communication with specified
     * remote host and port.
     *
     * @param  host The hostname of the server
     * @param  port The port to connect to
     * @return      0 if sucessful, negative integer otherwise
     */
    int server_connect(string host, string port);
    /**
     * Close the socket (if currently logged in to a
     * remote server, i.e. a socket exists)
     * @return 0 if sucessful, negative integer otherwise
     */
    int server_disconnect();
    /**
     * Print the author information and statement of academic integrity.
     */
    void author();
    /**
     * Print the external IP address of the current process.
     */
    void ip();
    /**
     * Print the port number that the current process is listening on.
     */
    void port();
    /**
     * Display a numbered list of all the currently logged-in clients. The
     * output displays the hostname, IP address, and the listening port numbers,
     * sorted by their listening port numbers, in increasing order. For example:
     *
     * 1 | stones.cse.buffalo.edu | 128.205.36.46 | 4545
     * 2 | embankment.cse.buffalo.edu | 128.205.36.35 |5000
     * 3 | highgate.cse.buffalo.edu | 128.205.36.33 | 5499
     * 4 | euston.cse.buffalo.edu | 128.205.36.34 | 5701
     */
    void list();

    int is_valid_ip(string ip);

    /**
     * Login to the server located at ip address: <server-ip> listening on port:
     * <server-port>.
     *
     * @param  host The hostname of the server
     * @param  port The port to connect to
     * @return      0 if sucessful, negative integer otherwise
     */
    void login(string host, string port);
    /**
     * Get an updated list of currently logged-in clients from the server.
     */
    void refresh();
    /**
     * Send message: <msg> to all logged-in clients. <msg> can have a maximum
     * length
     * of
     * 256 bytes and consists of valid ASCII characters.
     */
    void broadcast(string msg);
    /**
     * Block all incoming messages from the client with IP address: <client-ip>.
     * The
     * client
     * implementation notifies the server about this blocking.
     *
     * @throws InvalidIPException an invalid IP address was provided
     * @throws UnknownIPException an unknown IP address was provided
     * @throws DuplicateBanException the provided IP address is already blocked
     */
    void block_client(string ip);
    /**
     * Unblock a previously blocked client with IP address: <client-ip>. The
     * client
     * implementation notifies the server about the unblocking.
     *
     * @throws InvalidIPException an invalid IP address was provided
     * @throws UnknownIPException an unknown IP address was provided
     * @throws NoBanException the provided IP address is not blocked
     */
    void unblock_client(string ip);
    /**
     * Logout from the server. Note that the application is still able to accept
     * the
     * LOGIN, EXIT, and AUTHOR commands after logging out.
     */
    void logout();
    /**
     * Notify the user of a successfully executed command.
     * @param operation The command that was executed, e.g. LOGIN, AUTHOR, etc.
     * @param results   The resulting output from the executed command.
     */
    void notify_success(string operation, string results);
    /**
     * Notify the user that an error occured while executing a command.
     * @param operation The command that was executed, e.g. LOGIN, AUTHOR, etc.
     * @param error   The corresponding error message
     */
    void notify_error(string operation, string error);
    /**
     * Send a message to another client
     * @param ip  The IP address of the client
     * @param msg The message to send
     */
    void send_msg(string ip, string msg);
    /**
     * Send data to the server in the form of a string
     * @param  str The string to send
     * @return     0 on success, negative otherwise
     */
    int send_to_server(string str);

    void prompt_login();

  public:
    Client();
    ~Client();
    /**
     * Process a user inputted command.
     * @param cmd The user inputted string
     */
    void process_command(string cmd);
    /**
     * Launch a new client window.
     */
    void launch();
    /**
     * Exit client window.
     */
    void exit_client();
};

#endif