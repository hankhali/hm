// #include "Client.hpp"
#include "Server.hpp"

int main (int ac, char **av)
{
    int port;

    try {
        port = Server::parse(ac, av);

        Server server(port, av[2]);
    
        server.start();

    } catch (std::runtime_error &e) {
        Utils::printErr(e.what());
        return (EXIT_FAILURE);
    }
    return 0;
}

/*
    Team:

        SO_REUSEADDR
*/

/*
    *   ==================================================================
    *   ============================ Resources ===========================
    *   ==================================================================

    *   Http Article:         https://archive.ph/L1aGq#selection-1261.0-1269.30
    *   Beej guide link:      https://beej.us/guide/bgnet/pdf/bgnet_a4_c_1.pdf
*/

/*
    *   ==================================================================
    *   ============================== brew ==============================
    *   ==================================================================
    
    *   https://www.scivision.dev/macos-homebrew-non-sudo/
    *   cd goinfre
    *   mkdir ~/homebrew && curl -L https://github.com/Homebrew/brew/tarball/master | tar xz --strip 1 -C ~/homebrew
    *   open zshrc file:
            ~/.zshrc
    *   add this command with the alias section:
            alias brew='/Users/your intra name/goinfre/homebrew/bin/brew' 
    *   save the changes:
            source ~/.zshrc
    *   download irrsi:
            brew install irrsi

    *   ==================================================================
    *   ====================== nc Client Connection ======================
    *   ==================================================================
    
    *   nc localhost 8080 hi i'm deia
*/

/*
    *   ==================================================================
    *   ============================== Beej ==============================
    *   ==================================================================
        
    *   ============================ Sockets =============================
    *   What's Sockets?
        a way to speak to other programs using standard Unix file descriptors.
    *   What's file descriptor?
        . an integer associated with an open file.
        . that file can be a network connection, a FIFO, a pipe, a terminal.

    ......................................................................

    *   ======================== Type of sockets =========================
    *   Two Types of Internet Sockets?
        . Stream Sockets => “SOCK_STREAM”
        . Datagram Sockets => “SOCK_DGRAM”
    *   Stream Sockets?
        Stream sockets are (reliable) two-way connected communication streams. If you output two items into the socket in the order “1, 2”, they will arrive in the order “1, 2” at the opposite end.
    *   How do stream sockets achieve this high level of data transmission quality??
        They use a protocol called “The Transmission Control Protocol”, otherwise known as “TCP”, TCP makes sure your data arrives sequentially and error-free.
    *   TCP/IP?
        IP deals primarily with Internet routing and is not generally responsible for data integrity.
    *   Datagram Sockets?
        . sometimes called “connectionless sockets”, if you send a datagram, it may arrive. It may arrive out of order. If it arrives, the data within the packet will be error-free.
        . Datagram sockets also use IP for routing, but they don’t use TCP, they use the “User Datagram Protocol”
        . used for unreliable applications like games, audio, or video where if a few dropped packets here and there don’t mean the end of the Universe.
        . Why using it? because it's faster.
    *   Why Datagram Sockets are called connectionless??
        it’s because you don’t have to maintain an open connection as you do with stream sockets.

    ......................................................................
    
    *   ======================== Network Theory =========================
    *   Low level Network Theory?
        *   Data Encapsulation: (Ethernet(IP(UDP(TFTP(DATA)))))
        *   A layered model more consistent with Unix:
            • Application Layer (telnet, ftp, etc.)
            • Host-to-Host Transport Layer (TCP, UDP)
            • Internet Layer (IP and routing)
            • Network Access Layer (Ethernet, wi-fi, or whatever)
        *   capsulation:
            • All you have to do for stream sockets is send() the data out.
            • All you have to do for datagram sockets is encapsulate the packet in the method of your choosing and sendto() it out. 
            • The kernel builds the Transport Layer and Internet Layer on for you.
            • The hardware does the Network Access Layer.

    ......................................................................
    
    *   ================ IP Addresses, versions 4 and 6 =================
    *   IPv4 (32 bits): (i.e 192.168.1.1 => 11000000.10101000.00000001.00000001), (2^32 = 4.3 billion unique addresses)
    *   IPv6 (128 bits): (i.e 2001:0db8:85a3:0000:0000:8a2e:0370:7334), (2^128 = 3.4 x 10^38 unique addresses)

    ......................................................................
    
    *   ========================== Byte Order ===========================
    *   lets say we wanna store this the two-byte (16-bit) hex number, say b34f in the memory:
        • Big-Endian (Network Byte Order) stores the most significant byte first (b3 4f).
            In most network protocols (like TCP/IP), data is transmitted in Big-Endian format
        • Little-Endian (Host Byte Order) stores the least significant byte first (4f b3).
            Many modern computers, especially those using Intel or Intel-compatible processors, store data in Little-Endian format.

        This difference in byte ordering is important to keep in mind when working with data across different systems or architectures.

    *   The Solution: Conversion Functions:
        • htons() (Host to Network Short): Converts a 16-bit number from host byte order to network byte order.
        • htonl() (Host to Network Long): Converts a 32-bit number from host byte order to network byte order.
        • ntohs() (Network to Host Short): Converts a 16-bit number from network byte order to host byte order.
        • ntohl() (Network to Host Long): Converts a 32-bit number from network byte order to host byte order.

       These functions ensure that when sending data over a network, it's correctly formatted in Big-Endian, and when receiving data, it can be converted back to the host's Little-Endian format if necessary.

    *   There are two types of numbers that you can convert: 
        • short (two bytes, 16 bits, 2 ^ 16 = 65536) 
            -   Signed short range: -32,768 to 32,767
            -   Unsigned short range: 0 to 65,535
        • long (four bytes, 32 bits, 2 ^ 32 = 4294967296). 
            -   Signed long range: -2,147,483,648 to 2,147,483,647
            -   Unsigned long range: 0 to 4,294,967,295
    
    Basically, you’ll want to convert the numbers to Network Byte Order before they go out on the wire, and convert them to Host Byte Order as they come in off the wire.

    *   sockets?
        descriptors
    *   sockets?
        descriptors
    *   sockets?
        descriptors
    *   sockets?
        descriptors
    *   sockets?
        descriptors
    *   sockets?
        descriptors
    *   sockets?
        descriptors
*/
