#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <sys/socket.h>
#include <string>

class Client
{
private:
    int _client_fd;
    std::string _ip_address;
    bool _authenticated;
    std::string _nickname;
    std::string _username;

public:
    Client() : _client_fd(-1), _authenticated(false) {} // Initialize _client_fd to -1
    int GetFd() { return _client_fd; }
    void SetIpAddress(const std::string& ip_address) { _ip_address = ip_address; }
    void SetFd(int fd) { _client_fd = fd; }

    bool IsAuthenticated() const { return _authenticated; }
    void SetAuthenticated(bool auth) { _authenticated = auth; }

    void SetNickname(const std::string& nickname) { _nickname = nickname; }
    std::string GetNickname() const { return _nickname; }

    void SetUsername(const std::string& username) { _username = username; }
    std::string GetUsername() const { return _username; }

    void Connect(const std::string& ip, int port);
    void SendMessage(const std::string& message);
    void ReceiveMessage();
    void Close();

    


};

#endif
