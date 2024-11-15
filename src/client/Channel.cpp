#include "Channel.hpp"

Channel::Channel(const std::string& name) : _name(name) {}

Channel::~Channel() {}

void Channel::addClient(Client* client) {
    _clients.push_back(client);
}

void Channel::removeClient(Client* client) {
    _clients.erase(std::remove(_clients.begin(), _clients.end(), client), _clients.end());
}

void Channel::broadcastMessage(const std::string& message) {
    for (Client* client : _clients) {
        send(client->getFd(), message.c_str(), message.size(), 0);
    }
}

std::string Channel::getMemberList() const {
    std::string memberList;
    for (const Client* client : _clients) {
        if (!memberList.empty()) {
            memberList += " ";
        }
        memberList += client->getNickname();
    }
    return memberList;
}

#include "Channel.hpp"

// Constructor
Channel::Channel(const std::string& name) : _name(name) {}

// Destructor
Channel::~Channel() {}

// Add a client to the channel
void Channel::addClient(Client* client) {
    _clients.push_back(client);
}

// Remove a client from the channel
void Channel::removeClient(Client* client) {
    for (std::vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
        if (*it == client) {
            _clients.erase(it);
            break;
        }
    }
}

// Broadcast a message to all clients in the channel
void Channel::broadcastMessage(const std::string& message, Client* sender) {
    for (std::vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
        if (*it != sender) { // Do not send the message to the sender
            send((*it)->getFd(), message.c_str(), message.size(), 0);
        }
    }
}

// Get a list of nicknames of all clients in the channel
std::string Channel::getMemberList() const {
    std::string memberList;

    for (std::vector<Client*>::const_iterator it = _clients.begin(); it != _clients.end(); ++it) {
        if (!memberList.empty()) {
            memberList += " ";
        }
        memberList += (*it)->getNickname();
    }

    return memberList;
}

