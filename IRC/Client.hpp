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
public:
    Client() : _client_fd(-1) {} // Initialize _client_fd to -1
    int GetFd() { return _client_fd; }
    void SetIpAddress(const std::string& ip_address) { _ip_address = ip_address; }
    void SetFd(int fd) { _client_fd = fd; }

    void Connect(const std::string& ip, int port);
    void SendMessage(const std::string& message);
    void ReceiveMessage();
    void Close();
};

#endif
