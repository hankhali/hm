#ifndef __CLIENT_HPP__
#define __CLIENT_HPP__

#include <string>
#include <vector>

class Client {
private:
    int         _fd;               // File descriptor for the client socket
    std::string _nickname;         // Client's nickname
    std::string _username;         // Client's username
    std::string _realname;         // Client's real name
    bool        _registered;       // Is the client registered?
    std::string _buffer;           // Buffer for storing partial messages
    std::vector<std::string> _channels; // List of channels the client has joined

public:
    // Constructor and Destructor
    Client(int fd);
    ~Client();

    // Getters
    int         getFd() const;
    std::string getNickname() const;
    std::string getUsername() const;
    bool        isRegistered() const;
    std::string getBuffer() const;
    std::vector<std::string>& getChannels();

    // Setters
    void setNickname(const std::string& nickname);
    void setUsername(const std::string& username);
    void setRegistered(bool status);
    void appendToBuffer(const std::string& data);
    void clearBuffer();

    // Channel Management
    void addChannel(const std::string& channel);
    void removeChannel(const std::string& channel);
};

#endif // __CLIENT_HPP__
