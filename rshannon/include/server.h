#ifndef _SERVER_H_
#define _SERVER_H_

#define BUFFER_SIZE 256	// Size of receive buffer in bytes

class Server {
	private:
		struct Connection {
			int fd;
			std::string remote_ip;
		};
		Console* console;
		std::vector<Connection> active_connections;
		void *get_in_addr(struct sockaddr *sa);
		int init_socket(std::string port);
		int new_connection_handler(int listener);
		void process_data(int sockfd);
		void process_command();
	public:
		Server();
		~Server();
		int launch();
};

#endif