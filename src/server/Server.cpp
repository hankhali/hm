 #include "Server.hpp"
 #include "Client.hpp"

Server::Server(int port, const std::string& password) : _port(port), _password(password) {
    Utils::printMsg("Server object created", "yellow");
}

Server::~Server() {
    Utils::printMsg("Server object destroyed", "magenta");
}

int		Server::parse(int ac, char** av) {
	if (ac != 3) Utils::throwErr(AC_ERR);

	std::string 		portStr = av[1];
	std::string			password = av[2];
	std::stringstream 	ss;
	int 				port;

	ss << portStr;

	if (!(ss >> port) || !(ss.eof())) 
		Utils::throwErr(PORT_CHAR_ERR);
	// if (port < 1024 || port > 49151) 
	// 	Utils::throwErr(PORT_INT_ERR);
	if (password.empty()) 
		Utils::throwErr(EMPTY_PASSWORD_ERR);
	if (password.find_first_not_of(' ') == std::string::npos) 
		Utils::throwErr(SPACES_PASSWORD_ERR);
    if (password.size() > 5) 
        Utils::throwErr(PASSWORD_TOO_LONG_ERR);
	return (port);
}

void    Server::start() {
    try {
        initServer();

        // int poll(struct pollfd fds[], nfds_t nfds, int timeout);
        while (true) {
            if (poll(_fds.data(), _fds.size(), -1) < 0) 
                Utils::throwErr(POLL_FN_ERR);
            for (size_t i = 0; i < _fds.size(); ++i) {
                if (_fds[i].revents & POLLIN) 
                    _fds[i].fd == _listeningSocket ? acceptClient() : handleClient(_fds[i].fd);
            }
        }
    } catch (const std::exception& e) {
        Utils::throwErr(std::string(e.what()));
    }
}

void    Server::initServer() {
   try {
        Socket socket;

        _listeningSocket = socket.createSocket();
        socket.bindSocket(_listeningSocket, _port);

        // int listen(int sockfd, int backlog);
        listen(_listeningSocket, 10) < 0 
            ? Utils::throwErr(LISTEN_ERR)
            : addToPoll(_listeningSocket);

        Utils::printMsg("\nServer up & running on port -- " + Utils::toStr(_port) + " --\n", "green");
    } catch (const std::exception& e) {
        Utils::throwErr("Server initialization failed - " + std::string(e.what()));
    }
}

void    Server::addToPoll(int socket) {
    pollfd clientFd;

    clientFd.fd = socket;
    clientFd.events = POLLIN;
    _fds.push_back(clientFd);
}

void Server::acceptClient() {
    int clientSocket = accept(_listeningSocket, NULL, NULL);
    
    if (clientSocket < 0) {
        Utils::throwErr(ACCEPT_ERR);
    }

    addToPoll(clientSocket);

    // Create a new Client object and store it in the map
    _clients[clientSocket] = new Client(clientSocket);

    Utils::printMsg("New client connected: " + Utils::toStr(clientSocket), "blue");
}


void    Server::handleClient(int clientSocket) {
    char        buffer[1024];
    ssize_t     bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    std::string clientStrSocket = Utils::toStr(clientSocket);

    if (bytesRead <= 0) {
        Utils::printMsg("Client disconnected: " + clientStrSocket, "magenta");
        removeClient(clientSocket);
        return ;
    }

    buffer[bytesRead] = '\0';

    std::string message(buffer);    // Convert the buffer to a std::string
    std::string response = "Received message from client " + clientStrSocket + ": " + message;

    handleResponse(clientSocket, response);
}

void    Server::handleResponse(int clientSocket, const std::string& response) {
    Utils::printMsg(response, "green");
    send(clientSocket, response.c_str(), response.size(), 0);
}

void Server::removeClient(int clientSocket) {
    close(clientSocket);

    // Free the Client object
    if (_clients.find(clientSocket) != _clients.end()) {
        delete _clients[clientSocket];
        _clients.erase(clientSocket);
    }

    Utils::printMsg("Client removed: " + Utils::toStr(clientSocket), "magenta");
}

////////////////
void Server::parseCommand(Client* client, const std::string& command) {
    std::istringstream iss(command);
    std::string cmd;
    iss >> cmd;

    if (cmd == "NICK") {
        std::string nickname;
        iss >> nickname;
        handleNickCommand(client, nickname);
    } else if (cmd == "USER") {
        std::string username;
        iss >> username;
        handleUserCommand(client, username);
    } else if (cmd == "JOIN") {
        std::string channelName;
        iss >> channelName;
        handleJoinCommand(client, channelName);
    } else {
        // Handle unknown commands
        std::string error = ":server 421 " + client->getNickname() + " " + cmd + " :Unknown command\r\n";
        send(client->getFd(), error.c_str(), error.size(), 0);
    }
}

void Server::handleNickCommand(Client* client, const std::string& nickname) {
    // Placeholder for handling the NICK command
    client->setNickname(nickname);
    std::string response = ":server 001 " + nickname + " :Welcome to the IRC network\r\n";
    send(client->getFd(), response.c_str(), response.size(), 0);
}

void Server::handleUserCommand(Client* client, const std::string& username) {
    // Placeholder for handling the USER command
    client->setUsername(username);
    if (client->getNickname().empty()) {
        std::string error = ":server 431 :No nickname set\r\n";
        send(client->getFd(), error.c_str(), error.size(), 0);
    } else {
        std::string welcome = ":server 001 " + client->getNickname() + " :Welcome to the IRC network\r\n";
        send(client->getFd(), welcome.c_str(), welcome.size(), 0);
    }
}

void Server::handleJoinCommand(Client* client, const std::string& channelName) {
    // Placeholder for handling the JOIN command
    if (channelName.empty()) {
        std::string error = ":server 461 JOIN :Not enough parameters\r\n";
        send(client->getFd(), error.c_str(), error.size(), 0);
        return;
    }
    // Simulate adding the client to a channel
    std::string response = ":server 332 " + client->getNickname() + " JOIN " + channelName + "\r\n";
    send(client->getFd(), response.c_str(), response.size(), 0);
}



/*  
    *   == Explanation ==

    *   parse:
        
        This function parses the command-line arguments passed to the server program. It checks the number of arguments, validates the port number, we make sure it's a number and within range, and ensures that the password is not empty or only spaces.

    *   createListeningSocket:

        This function creates a socket, binds it to the specified port, and puts the server into a listening state to accept new client connections. The socket is not used to transfer data; it is used solely for accepting new connections.

    *   initServer:

        Initializes the server by calling createListeningSocket() and handles any errors that may occur during initialization.

    *   addToPoll:

        Add a client socket to the list of monitored file descriptors (_fds) for the poll() system call. The server uses poll() to monitor both the listening socket (for new connections) and existing client sockets (for incoming data).
   
    *   acceptClient:

        Accepts a new incoming client connection, creates a socket for communication with the client, and adds the client socket to the list of monitored file descriptors (poll list).
    
        The client is also added to the _clients map for tracking purposes.
   
        struct pollfd {
            int fd;        // File descriptor (the socket you're monitoring)
            short events;  // The events you're interested in (e.g., readable, writable)
            short revents; // The events that actually occurred (set by poll())
        };

        The pollfd structure is a key part of the poll() system call, which allows the server to handle multiple client connections concurrently, without blocking on any one connection.

        1- pollfd clientFd;
            pollfd Structure: pollfd is a structure used by the poll() system call to represent a file descriptor (in this case, a socket) and the events that you want to monitor on that file descriptor.

        2- clientFd.fd = clientSocket;
            Assigning the File Descriptor: Here, you are assigning the new client socket (clientSocket) to the fd field of the pollfd structure. This is the socket that was just accepted with the accept() system call.
            
            Why?: This is necessary so that poll() can monitor this specific socket for events such as incoming data from the client. Each client gets its own pollfd entry.

        3- clientFd.events = POLLIN;
            Monitoring for Incoming Data (POLLIN): The events field of pollfd specifies what kind of events you are interested in monitoring on the clientSocket.

            POLLIN: This flag means you're interested in checking if there's data available to read on this socket. In the context of networking, this means that the client has sent some data, and your server should be ready to read that data.
            
            Other possible flags include:
                POLLOUT: Check if the socket is ready for writing.
                POLLERR: Check if an error occurred on the socket.
                But in this case, you're only interested in POLLIN because you want to know when the client has sent data that the server should handle.

        4- _fds.push_back(clientFd);
            Adding the Client to the Poll List: _fds is a vector of pollfd structures. Each pollfd structure represents a socket (or file descriptor) that the server is monitoring for events.

                push_back(clientFd): This line adds the newly accepted client to the list of sockets that the server will monitor using poll().

                When poll() is called, it will now monitor this new client socket (along with any other sockets in _fds) to see if any events (such as incoming data) occur.
                
                Why a vector?: The vector allows you to dynamically manage multiple client connections. As new clients connect, they are added to _fds, and as clients disconnect, they are removed.

        5- _clients[clientSocket] = ""; 
            Tracking Client Data: _clients is likely a std::map<int, std::string>, where the key is the client socket (an int), and the value is a string representing data related to that client.
           
                Placeholder for Data: In this case, you’re initializing the entry in _clients with an empty string. This could be a placeholder for storing data associated with each client, such as:
                    1- Partial messages (if the client sends fragmented data).
                    2- Client-specific information (like username, nickname, etc.).

                Why?: This map allows you to keep track of multiple clients, associating each client socket with its own state or data. Each client gets its own unique clientSocket, which is used as the key in this map.

                what's the raw pointer?
                    A raw pointer in C++ is a pointer that holds the memory address of another variable or object.

                    The raw pointer is used to access the memory address of the client socket, which is an integer value. This allows you to uniquely identify each client connection and associate it with any relevant data or state.

    *   handleClient:

        Handles communication with a connected client by receiving data from the client, printing the received message, and sending a response (simple echo).
    
        If the client disconnects or an error occurs, the client is removed.
    
    *   handleResponse:

        Prints the received message from the client and sends a response back to the client.

    *   removeClient:

        Closes the client socket, removes the client from the _clients map, and prints a message indicating that the client was removed.
    
    *   start:

        This is the main server loop that uses poll() to monitor both the server's listening socket (for new connections) and existing client sockets (for incoming data).
   
        It continuously checks for events and calls either acceptClient() to handle new connections or handleClient() to process client data.

        serverFd.fd = _listeningSocket;
        serverFd.events = POLLIN;
        _fds.push_back(serverFd);

            Monitoring the Listening Socket:
                You create a pollfd structure (serverFd) to represent the listening socket (_listeningSocket). The listening socket is responsible for accepting new client connections.

                You set serverFd.fd to the listening socket's file descriptor (_listeningSocket) so that poll() can monitor this socket for incoming connection requests.

                serverFd.events = POLLIN;: This means you want poll() to watch this socket for the POLLIN event, which occurs when there’s data available to read (in this case, when a client is trying to connect to the server).

                _fds.push_back(serverFd);: The _fds vector contains all the file descriptors you want to monitor. Here, you add the listening socket to this list.

        After this, poll() will be able to detect when a new client connection is available by monitoring the listening socket.

        why.data ?
            because _fds is a std::vector<pollfd>, and the poll() function requires a raw pointer (a C-style array) to an array of pollfd structures. The .data() method of std::vector returns a pointer to the underlying array that stores the elements of the vector, which is exactly what poll() expects.

            the .data method returns a pointer to the first element of the vector’s underlying array.

            _fds.data(), gives the address of the first element of the std::vector<pollfd>.
            
        for loop for (size_t i = 0; i < _fds.size(); ++i)
            The for loop iterates over all the file descriptors and check for readable events (POLLIN) on each socket (either the listening socket or any of the client sockets) and handle them appropriately

            if (_fds[i].revents & POLLIN):
                Purpose: This checks if the current socket (in _fds[i]) has the POLLIN event set in the revents field. The revents field tells you which events have occurred on the socket since the last poll() call.
                    
                POLLIN: This event indicates that there is data to be read on the socket. For the listening socket, this means that a client is trying to connect. For client sockets, it means the client has sent data that the server needs to read.
            
                Bitwise AND (&): The expression _fds[i].revents & POLLIN checks if the POLLIN bit is set in revents. If it is, it means that this socket is ready to read (either new client connections or incoming client data).

    *   Why using pollfd in both function, start() and acceptClient()?
            In start(): You need to add the listening socket to the pollfd list so that the server can detect when a new client is attempting to connect.
        
            In acceptClient(): You need to add the new client socket to the pollfd list so that the server can detect when this client sends data that needs to be processed.
*/

/*
    *   == Man ==

    *   listen: int listen(int sockfd, int backlog);

        The listen() function is used to put a socket in the listening state, allowing it to accept incoming connections. The second argument specifies the maximum number of pending connections that can be queued for the socket.

    *   accept: int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

        The accept() function is used to accept a new incoming connection on a listening socket. It returns a new socket descriptor for the accepted connection, which can be used for communication with the client.

        struct sockaddr *addr: A pointer to a sockaddr structure that will contain the address of the client.

        socklen_t *addrlen: A pointer to a socklen_t variable that should be initialized to the size of the sockaddr structure before calling accept(). 

        Since your current implementation doesn’t require the client’s address or port (for logging or any other reason), it’s fine to pass NULL for both parameters.

    *   recv: ssize_t recv(int sockfd, void *buf, size_t len, int flags);

        The recv() function is used to receive data from a connected socket. It reads data from the socket into the buffer pointed to by buf, up to a maximum of len bytes.

        ssize_t: The return type of recv() is ssize_t, which is a signed integer type that can represent the number of bytes read or an error code.

        void *buf: A pointer to the buffer where the received data will be stored.

        size_t len: The maximum number of bytes to read into the buffer.

        int flags: Additional flags that can be used to control the behavior of the recv() function. In your case, you’re using 0, which means no special flags are set.

    *   poll: int poll(struct pollfd fds[], nfds_t nfds, int timeout);

        The poll() function is used to monitor file descriptors for events. It waits for one of the specified events to occur on any of the file descriptors in the list and returns the number of file descriptors with events.

        If an event occurs, the revents field of the pollfd structure is set to indicate the type of event that occurred.

        The poll() function can be used to monitor multiple file descriptors simultaneously, making it suitable for handling multiple clients in a server application.

        Arguments of poll():
            _fds.data(): This provides the array of pollfd structures (from the _fds vector) that poll() will monitor.

                _fds contains both the listening socket (for accepting new connections) and any connected client sockets (for receiving data).

            _fds.size(): The number of file descriptors to monitor. This tells poll() how many file descriptors are in the _fds vector.

            -1: The timeout value. A -1 value means that poll() will block indefinitely, waiting until an event occurs on one of the monitored sockets.

        Return Value:
            poll() returns the number of file descriptors where an event occurred (e.g., data available to read).

            If poll() returns a negative value, it indicates an error, so the Utils::throwErr(POLL_FN_ERR) is triggered to handle the error.
        
        Blocking and Efficiency:
            Blocking: poll() will block the server process and wait until an event occurs on any of the monitored file descriptors. This allows the server to use non-blocking I/O and only act when there's work to do, without busy-waiting.
            
            Efficiency: poll() allows you to handle multiple clients simultaneously in a single thread by waiting for events on multiple file descriptors at once.
*/

/*
    *   == Summary ==

    *   pollfd in start(): Monitors the listening socket for new connections.
    *   pollfd in acceptClient(): Monitors the client sockets for incoming data after a connection is established.
    *   poll(): Allows the server to handle multiple connections efficiently by checking for events across all sockets (both the listening socket and client sockets) without blocking on any single socket.
*/
