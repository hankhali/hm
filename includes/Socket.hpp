#ifndef __SOCKET_HPP__
# define __SOCKET_HPP__

#include "Utils.hpp"

class Socket
{
    public:
        Socket();
        ~Socket();

        int     createSocket();
        void    bindSocket(int serverSocket, int port);
};


#endif // __SOCKET_HPP__