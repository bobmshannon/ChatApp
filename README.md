# ChatApp
For like, chatting with other people and stuff.

# Running
*To run as a server listening on port 4322
`./chat_app s 4322` 
*To run as a client listening on port 4322
`./chat_app c 4322`

# Available Commands
## IP
Print the IP address of the process.
## PORT
Print the port number the process is listening on.
## LIST
Display a numbered list of all the currently logged-in clients
## STATISTICS
Display a numbered list of all the clients that have ever logged-in to the server and statistics about each one. Includes number of messages sent and received, and online/offline status
## BLOCKED `<client-ip>`
Display a numbered list of all the clients blocked by the client with ip address: `<client-ip>`.
## LOGIN <server-ip> <server-port>
This command is used by a client to login to the server located at ip address: `<server-ip>` listening on port: `<server-port>`.
## REFRESH
Get an updated list of currently logged-in clients from the server.
## SEND `<client-ip>` `<msg>`
Send message: `<msg>` to client with ip address: `<client-ip>`.
## BROADCAST `<msg>`
Send message: <msg> to all logged-in clients.
## BLOCK `<client-ip>`
Block all incoming messages from the client with IP address: <client-ip>.
## UNBLOCK `<client-ip>`
Unblock a previously blocked client with IP address: `<client-ip>`.
## LOGOUT
Logout from the server.
## EXIT
Logout from the server (if logged-in) and terminate the application with exit code 0.

