#ifndef _CLIENT_H_
#define _CLIENT_H_

class Client {
	private:
		Console* console;
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