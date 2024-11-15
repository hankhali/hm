#ifndef __CHANNEL_HPP__
#define __CHANNEL_HPP__

#include <string>
#include <vector>
#include <algorithm>
#include "Client.hpp"

class Channel {
private:
    std::string _name;                // Channel name
    std::vector<Client*> _clients;    // List of clients in the channel

public:
    // Constructor and Destructor
    Channel(const std::string& name);
    ~Channel();

    // Methods
    void addClient(Client* client);
    void removeClient(Client* client);
    void broadcastMessage(const std::string& message, Client* sender = NULL);
    std::string getMemberList() const; // Returns a space-separated list of client nicknames

    // Getter
    const std::string& getName() const { return _name; }
};

#endif // __CHANNEL_HPP__
