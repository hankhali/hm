#ifndef __UTILS_HPP__
# define __UTILS_HPP__

#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>

#include <sys/socket.h>
#include <netinet/in.h>
#include <map>
#include <vector>
#include <arpa/inet.h>
#include <poll.h>
#include <unistd.h>

#define AC_ERR "Invalid number of arguments - Usage: ./ircserv <port> <password>"
#define PORT_CHAR_ERR "Invalid port number - Port must be a number"
#define PORT_INT_ERR "Invalid port number - Port number should be between 1024 and 49151"
#define EMPTY_PASSWORD_ERR "Password cannot be empty"
#define SPACES_PASSWORD_ERR "Password cannot be only spaces"
#define PASSWORD_TOO_LONG_ERR "Password is too long - Max length is 5 characters"
#define SOCKET_ERR "Socket creation failed"
#define BIND_ERR "Socket binding failed"
#define LISTEN_ERR "Listening failed"
#define POLL_FN_ERR "Poll failed"
#define ACCEPT_ERR "Client accept failed"
#define READ_ERR "Read failed"
#define WRITE_ERR "Write failed"
#define CLOSE_ERR "Close failed"

class Utils 
{
    public:
        static void         printMsg(const std::string& msg, const std::string& color);
        static void         printErr(const std::string& errMsg);
        static void         throwErr(const std::string& errMsg);

        template <typename T>
        static std::string  toStr(T value);
};

template <typename T>
std::string Utils::toStr(T value) {
    std::stringstream ss;

    ss << value;
    return ss.str();
}

#endif

/*
    *   #define PORT_INT_ERR "Invalid port number - Port number should be between 1024 and 49151"
        
        Port Number Ranges (0 - 65535):
        - Well Known Ports: 0 - 1023
            -   Port numbers between 0 and 1023 are reserved for well-known services and protocols. 
            -   Such as HTTP on port 80, HTTPS on port 443, FTP on port 21, etc..
            -   These ports are typically used by system-level processes or specific network services.
        - Registered Ports: 
            -   Ports 1024 to 49151 are known as registered ports. 
            -   These ports can be used by user applications for specific network services or communication.
            -   These ports are not reserved for any specific service.
            -   The "safe" range for most user-level applications.
        - Dynamic/Private Ports: 49152 - 65535
            -   Ports 49152 to 65535 are known as dynamic or private ports.
            -   These ports are typically used for temporary or ephemeral purposes like client-side communication. When an application opens a connection to a server, the system will often use a port from this range for the client side of the connection.
            -   While it's technically possible to bind a server to one of these ports, it's generally not recommended because they are often dynamically assigned by the operating system for client-side communication.
*/
