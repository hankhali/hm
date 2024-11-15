#include "Socket.hpp"

Socket::Socket() {
    Utils::printMsg("Socket object created", "yellow");
}

Socket::~Socket() {
    Utils::printMsg("Socket object destroyed", "magenta");
}

int     Socket::createSocket() {
    // int socketFd = socket(domain, type, protocol);
    int socketFd = socket(AF_INET, SOCK_STREAM, 0);

    socketFd < 0 
        ? Utils::throwErr(SOCKET_ERR) 
        : Utils::printMsg("Socket created successfully", "blue");

    return socketFd;
}

void    Socket::bindSocket(int socketFd, int port) {
    struct sockaddr_in  serv_addr;  // sock address internet to be used with IPv4.

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);

    //  int bind(int socket, const struct sockaddr *address, socklen_t address_len);
    int bindFlag = bind(socketFd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));

    bindFlag < 0 
        ? Utils::throwErr(BIND_ERR) 
        : Utils::printMsg("Socket bound to port " + Utils::toStr(port), "blue");
}

/*
    * What is socket?
        - It's a file descriptor. 
        - A socket is an endpoint that enables two processes to communicate with each other, either on the same machine or across a network.
        - It acts as an interface between the application layer and the transport layer of the networking stack.

    * Programming with TCP/IP sockets
        There are a few steps involved in using sockets:
            1-  Create the socket
            2-  Identify the socket
            3-  On the server, wait for an incoming connection
            4-  Send and receive messages
            5-  Close the socket

    * Step 1. Create a socket -- (#include <sys/socket.h>)
        int Socket::createSocket()
        { 
            int socketFd = socket(domain, type, protocol);
            All the parameters as well as the return value are integers:

                Domain: AF_INET (IPv4 protocol)
                    communication domain in which the socket should be created. Some of address families are AF_INET (IP), AF_INET6 (IPv6), AF_UNIX (local channel, similar to pipes), AF_ISO (ISO protocols), and AF_NS (Xerox Network Systems protocols).

                    AF_INET (ipv4) or AF_INET6 (ipv6) or AF_UNSPEC (doesn't matter)

                Type: SOCK_STREAM (TCP)
                    type of service. This is selected according to the properties required by the application: SOCK_STREAM (virtual circuit service), SOCK_DGRAM (datagram service), SOCK_RAW (direct IP service).

                Protocol: 0 (IP)
                    protocol to be used with the socket. This is usually set to 0 to use the default protocol for the given socket type (IPPROTO_TCP for SOCK_STREAM, IPPROTO_UDP for SOCK_DGRAM).
        }

        For TCP/IP sockets, we want to specify the IP address family (AF_INET) and virtual circuit service (SOCK_STREAM). Since there’s only one form of virtual circuit service, there are no variations of the protocol, so the last argument, protocol, is zero.

        AF_INET: Specifies the address family. AF_INET is used for IPv4 protocols.
        SOCK_STREAM: Indicates that the socket type is for a reliable, connection-oriented service (TCP).
        0: Protocol parameter, set to 0 to use the default protocol for the given socket type (IPPROTO_TCP for SOCK_STREAM).

    * Step 2. Identify (name) a socket -- (#include <netinet/in.h>)
        When we talk about naming a socket, we are talking about assigning a transport address to the socket (a port number in IP networking). In sockets, this operation is called binding an address and the bind system call is used for this.

        void Socket::bindSocket(int socketFd, int port)
        {
            struct sockaddr_in serv_addr;
            serv_addr.sin_family = AF_INET;
            serv_addr.sin_addr.s_addr = INADDR_ANY;
            serv_addr.sin_port = htons(port);

            if (bind(socketFd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
                Utils::throwErr(BIND_ERR);
        }

        int bind(int socket, const struct sockaddr *address, socklen_t address_len);

        -   The first parameter: 
            socket, is the socket that was created with the socket system call.
        -   For the second parameter:
            the structure sockaddr is a generic container that just allows the OS to be able to read the first couple of bytes that identify the address family. The address family determines what variant of the sockaddr struct to use that contains elements that make sense for that specific communication type. For IP networking, we use struct sockaddr_in, which is defined in the header netinet/in.h. This structure defines:
            struct sockaddr_in 
            { 
                __uint8_t         sin_len; 
                sa_family_t       sin_family; 
                in_port_t         sin_port; 
                struct in_addr    sin_addr; 
                char              sin_zero[8]; 
            };

            Before calling bind, we need to fill out this structure. The three key parts we need to set are:
                -   sin_family: The address family, which is AF_INET for IP networking.
                -   sin_addr: 
                    The IP address of the host. We use INADDR_ANY (0.0.0.0) to allow the socket to accept connections from any client. 
                    The address for this socket. This is just your machine’s IP address. With IP, your machine will have one IP address for each network interface. For example, if your machine has both Wi-Fi and ethernet connections, that machine will have two addresses, one for each interface. Most of the time, we don’t care to specify a specific interface and can let the operating system use whatever it wants. The special address for this is 0.0.0.0, defined by the symbolic constant INADDR_ANY.
                -   sin_port: 
                    The port number. We use the htons function to convert the port number from host byte order to network byte order. 
                    The port number (the transport address). You can explicitly assign a transport address (port) or allow the operating system to assign one. If you’re a client and won’t be receiving incoming connections, you’ll usually just let the operating system pick any available port number by specifying port 0. If you’re a server, you’ll generally pick a specific number since clients will need to know a port number to connect to.

        -   Te third parameter:
            specifies the length of that structure, since the address structure may differ based on the type of transport used, this is simply sizeof(struct sockaddr_in).

        The bind() function associates the socket with:
        -   a specific IP address  
        -   port number. 

        The IP address is set to INADDR_ANY, which allows the socket to accept connections from any client. 
        The port number is set to the value passed as an argument to the function. 
        The htons() function converts the port number from host byte order to network byte order.

        The bind() function returns 0 on success and -1 on failure. 
*/
