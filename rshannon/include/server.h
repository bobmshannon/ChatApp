#ifndef _SERVER_H_
#define _SERVER_H_


#define LISTEN_PORT "3490"  // the port users will be connecting to
#define BACKLOG 10	 // how many pending connections queue will hold

class Server {
	private:
		Console* console;
		static void sigchld_handler(int s);
		void *get_in_addr(struct sockaddr *sa);
		int init_socket(std::string port);
		int new_connection_handler(int listener);
	public:
		Server();
		~Server();
		int launch();
};

#endif