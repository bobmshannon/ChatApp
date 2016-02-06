#ifndef _CLIENT_H_
#define _CLIENT_H_

class Client {
	private:
		Console* console;
	public:
		Client();

		~Client();
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