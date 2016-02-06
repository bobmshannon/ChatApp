#ifndef COMMANDS_H_
#define COMMANDS_H_

namespace commands {
/**
 * Print the external IP address of the current process.
 */
void ip();

/**
 * Print the author information and statement of academic integrity.
 */
void author();

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
 * @throws InvalidIPException an invalid IP address or port number was provided
 * @throws ConnectionErrorException an unreachable IP address or port number
 * combination was provided
 */
void login();

/**
 * Get an updated list of currently logged-in clients from the server.
 */
void refresh();

/**
 * Send message: <msg> to client with ip address: <client-ip>. <msg>
 * can have a maximum length of 256 bytes and consists of valid ASCII
 * characters.
 *
 * @throws InvalidIPException an invalid IP address was provided
 * @throws UnknownIPException an unknown IP address was provided
 */
void send();

/**
 * Send message: <msg> to all logged-in clients. <msg> can have a maximum length
 * of
 * 256 bytes and consists of valid ASCII characters.
 */
void broadcast();

/**
 * Block all incoming messages from the client with IP address: <client-ip>. The
 * client
 * implementation notifies the server about this blocking.
 *
 * @throws InvalidIPException an invalid IP address was provided
 * @throws UnknownIPException an unknown IP address was provided
 * @throws DuplicateBanException the provided IP address is already blocked
 */
void block();

/**
 * Unblock a previously blocked client with IP address: <client-ip>. The client
 * implementation notifies the server about the unblocking.
 *
 * @throws InvalidIPException an invalid IP address was provided
 * @throws UnknownIPException an unknown IP address was provided
 * @throws NoBanException the provided IP address is not blocked
 */
void unblock();

/**
 * Display a numbered list of all the clients (see BLOCK command in section 5.6)
 * blocked by the client with ip address: <client-ip>. The output displays the
 * hostname, IP address, and the listening port numbers, sorted by their
 * listening
 * port numbers, in increasing order. The output format is identical to that of
 * the
 * LIST command.
 *
 * @throws InvalidIPException an invalid IP address was provided
 * @throws UnknownIPException an unknown IP address was provided
 */
void blocked();

/**
 * Logout from the server. Note that the application is still able to accept the
 * LOGIN, EXIT, and AUTHOR commands after logging out.
 */
void logout();

/**
 * Logout from the server if logged in, and then terminate the application.
 */
void exit();

/**
 * Display a numbered list of all the clients that have ever logged-in to the
 * server
 * and statistics about each one.
 *
 * The output displays the hostname, #messages-sent, #messages-received, and the
 * current
 * status: online/offline depending on whether the client is currently logged-in
 * or not,
 * sorted by their listening port numbers, in increasing order. For example:
 * 1 | stones.cse.buffalo.edu | 4 | 0 | online
 * 2 | embankment.cse.buffalo.edu | 3 | 67 | offline
 * 3 | highgate.cse.buffalo.edu | 7 | 14 | online
 * 4 | euston.cse.buffalo.edu | 11 | 23 | online
 *
 * Note that the statistics counters are not reset on client logout.
 */
void statistics();
}

#endif