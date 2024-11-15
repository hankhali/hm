#include "Client.hpp"
#include <algorithm>

Client::Client(int fd) : _fd(fd), _registered(false) {}

Client::~Client() {}

// Getters
int Client::getFd() const { return _fd; }

std::string Client::getNickname() const { return _nickname; }

std::string Client::getUsername() const { return _username; }

bool Client::isRegistered() const { return _registered; }

std::string Client::getBuffer() const { return _buffer; }

std::vector<std::string>& Client::getChannels() { return _channels; }

// Setters
void Client::setNickname(const std::string& nickname) { _nickname = nickname; }

void Client::setUsername(const std::string& username) { _username = username; }

void Client::setRegistered(bool status) { _registered = status; }

void Client::appendToBuffer(const std::string& data) { _buffer += data; }

void Client::clearBuffer() { _buffer.clear(); }

// Channel Management
void Client::addChannel(const std::string& channel) {
    if (std::find(_channels.begin(), _channels.end(), channel) == _channels.end()) {
        _channels.push_back(channel);
    }
}

void Client::removeChannel(const std::string& channel) {
    _channels.erase(std::remove(_channels.begin(), _channels.end(), channel), _channels.end());
}
