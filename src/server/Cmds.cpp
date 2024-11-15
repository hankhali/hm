#include "Server.hpp"
#include "Client.hpp"

void Server::handleNickCommand(Client* client, const std::string& nickname) 
{
    if (nickname.empty()) 
    {
        std::string error = ":server 431 :No nickname given\r\n";
        send(client->getFd(), error.c_str(), error.size(), 0);
        return;
    }

    for (std::map<int, Client*>::const_iterator it = _clients.begin(); it != _clients.end(); ++it) 
    {
        Client* otherClient = it->second;
        if (otherClient->getNickname() == nickname) 
        {
            std::string error = ":server 433 " + nickname + " :Nickname is already in use\r\n";
            send(client->getFd(), error.c_str(), error.size(), 0);
            return;
        }
    }

    client->setNickname(nickname);
    std::string response = ":server 001 " + nickname + " :Welcome to the IRC network\r\n";
    send(client->getFd(), response.c_str(), response.size(), 0);
}



void handleUserCommand(Client* client, const std::string& username) 
{
    if (username.empty()) 
    {
        std::string error = ":server 461 USER :Not enough parameters\r\n";
        send(client->getFd(), error.c_str(), error.size(), 0);
        return;
    }
    client->setUsername(username);
    if (!client->getNickname().empty()) 
    {
        std::string welcome = ":server 001 " + client->getNickname() + " :Welcome to the IRC network\r\n";
        send(client->getFd(), welcome.c_str(), welcome.size(), 0);
    }
}

void Server::handleJoinCommand(Client* client, const std::string& channelName) {
    // Check for missing parameters
    if (channelName.empty()) {
        std::string error = ":server 461 JOIN :Not enough parameters\r\n";
        send(client->getFd(), error.c_str(), error.size(), 0);
        return;
    }

    // Check if the channel already exists, if not, create it
    if (_channels.find(channelName) == _channels.end()) {
        _channels[channelName] = new Channel(channelName);
    }

    // Add the client to the channel
    Channel* channel = _channels[channelName];
    channel->addClient(client);

    // Notify the channel that the client has joined
    std::string joinMessage = ":" + client->getNickname() + " JOIN :" + channelName + "\r\n";
    channel->broadcastMessage(joinMessage, client);

    // Send the client the current list of members in the channel
    std::string response = ":server 353 " + client->getNickname() + " = " + channelName + " :" + channel->getMemberList() + "\r\n";
    response += ":server 366 " + client->getNickname() + " " + channelName + " :End of /NAMES list\r\n";
    send(client->getFd(), response.c_str(), response.size(), 0);
}

void Server::handlePrivmsgCommand(Client* client, const std::string& target, const std::string& message) {
    // Check for missing parameters
    if (target.empty() || message.empty()) {
        std::string error = ":server 412 :No text to send\r\n";
        send(client->getFd(), error.c_str(), error.size(), 0);
        return;
    }

    if (target[0] == '#') { // Handle channel messages
        // Check if the channel exists
        if (_channels.find(target) == _channels.end()) {
            std::string error = ":server 403 " + target + " :No such channel\r\n";
            send(client->getFd(), error.c_str(), error.size(), 0);
            return;
        }

        // Broadcast the message to the channel
        Channel* channel = _channels[target];
        std::string broadcastMessage = ":" + client->getNickname() + " PRIVMSG " + target + " :" + message + "\r\n";
        channel->broadcastMessage(broadcastMessage, client);

    } else { // Handle private messages
        bool found = false;

        // Iterate through the clients to find the target
        for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
            Client* otherClient = it->second; // Access the client object
            if (otherClient->getNickname() == target) {
                std::string privmsg = ":" + client->getNickname() + " PRIVMSG " + target + " :" + message + "\r\n";
                send(otherClient->getFd(), privmsg.c_str(), privmsg.size(), 0);
                found = true;
                break;
            }
        }

        // If no client with the target nickname is found, send an error
        if (!found) {
            std::string error = ":server 401 " + target + " :No such nick/channel\r\n";
            send(client->getFd(), error.c_str(), error.size(), 0);
        }
    }
}
