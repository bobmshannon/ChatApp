#ifndef _SERVER_H_
#define _SERVER_H_

#define BUFFER_SIZE 256	// Size of receive buffer in bytes
#define MESSAGE_SIZE 256

using std::string;
using std::vector;

class Server {
	private:
		struct Connection {
			int fd;
			string remote_ip;
			bool active;
		};
		Console* console;
		vector<Connection> active_connections;
		void *get_in_addr(struct sockaddr *sa);
		/**
		 * Initialize a new socket on specified port
		 * @param  port The port to listen on
		 * @return      A valid file descriptor > 0, negative otherwise
		 */
		int init_socket(string port);
		/**
		 * Handle a new connection from a client by initializing a new socket.
		 * @param  listener The file descriptor corresponding to the socket listening for new connections
		 * @return          A valid file descriptor > 0, negative otherwise
		 */
		int new_connection_handler(int listener);
		/**
		 * Processing incoming data string from client.
		 * @param sockfd The file descriptor the data was read from 
		 * @param data   The data that was read
		 */
		void process_data(int sockfd, string data);
		/**
		 * Send data to the client in the form of a string.
		 * @param  str      The string to send
		 * @param  clientfd The file descriptor corresponding to the appropriate socket
		 * @return          0 success, negative otherwise
		 */
		int send_to_client(string str, int clientfd);
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