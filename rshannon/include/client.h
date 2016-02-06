#ifndef _CLIENT_H_
#define _CLIENT_H_

class Client {
  private:
    Console* console;
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
    /**
     * Login to the server located at ip address: <server-ip> listening on port:
     * <server-port>.
     *
     * @throws InvalidIPException an invalid IP address or port number was
     * provided
     * @throws ConnectionErrorException an unreachable IP address or port number
     * combination was provided
     */
    void login();
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
    void broadcast();
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
    void block();
    /**
     * Unblock a previously blocked client with IP address: <client-ip>. The
     * client
     * implementation notifies the server about the unblocking.
     *
     * @throws InvalidIPException an invalid IP address was provided
     * @throws UnknownIPException an unknown IP address was provided
     * @throws NoBanException the provided IP address is not blocked
     */
    void unblock();
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
    void notify_success(std::string operation, std::string results);

  public:
    Client();
    ~Client();
    /**
     * Process a user inputted command.
     * @param cmd The user inputted string
     */
    void process_command(std::string cmd);
    /**
     * Launch a new client window.
     */
    void launch();
    /**
     * Exit client window.
     */
    void exit();
};

#endif