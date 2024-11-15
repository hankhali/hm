#ifndef __SERVER_HPP__
# define __SERVER_HPP__

#include "Socket.hpp"
#include "Client.hpp" // Include Client class
#include "Channel.hpp" // Include Channel class (if it exists)

#include <map>
#include <string>
#include <vector>
#include <poll.h>

class Server
{
	private:
		int								_port;                  // Server port
		int								_listeningSocket;       // Listening socket for incoming connections
		std::string						_password;              // Server password
		std::map<int, Client*>			_clients;               // Map of client sockets to Client objects
		std::map<std::string, Channel*>	_channels;              // Map of channel names to Channel objects
		std::vector<pollfd> 			_fds;                   // Vector to store pollfd structs for monitoring

		// Private Methods
		void createListeningSocket();                                   // Create a socket and bind it to the specified port
		void initServer();                                              // Initialize server socket
		void addToPoll(int socket);                                     // Add a client socket to the pollfd vector
		void acceptClient();                                            // Accept new clients and add them to the clients map
		void handleClient(int clientSocket);                            // Read data from clients and respond accordingly
		void handleResponse(int clientSocket, const std::string& response); // Handle client responses
		void removeClient(int clientSocket);                            // Remove clients from the server

		// Command Handling
		void parseCommand(Client* client, const std::string& command);  // Parse and execute client commands
		void handleNickCommand(Client* client, const std::string& nickname);  // Handle NICK command
		void handleUserCommand(Client* client, const std::string& username);  // Handle USER command
		void handleJoinCommand(Client* client, const std::string& channelName); // Handle JOIN command
		void handlePrivmsgCommand(Client* client, const std::string& target, const std::string& message); // Handle PRIVMSG command

	public:
		// Constructor and Destructor
		Server(int port, const std::string& password);                 // Constructor
		~Server();                                                     // Destructor			

		// Public Methods
		void start();                                                  // Main loop using poll() to handle multiple clients
		static int parse(int ac, char** av);                           // Parse server inputs
};

#endif // __SERVER_HPP__
