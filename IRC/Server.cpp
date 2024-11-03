/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamone <pamone@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 12:31:50 by pamone            #+#    #+#             */
/*   Updated: 2024/11/03 02:13:02 by pamone           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Client.hpp"
#include <cstddef>
#include <cstdlib>
#include <exception>
#include <fcntl.h>
#include <iostream>
#include <iterator>
#include <netinet/in.h>
#include <stdexcept>
#include <sys/poll.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
bool Server::_signal = false;

void Server::SignalHandler(int signum)
{
    (void)signum;

    std::cout << "The signal Received is " << signum << std::endl;
    Server::_signal = true;
}

void Server::CloseFds()
{
    for(size_t i(0); i < _clients.size(); i++)
    {
         std::cout << "Closing the Client FD" << std::endl;
        close(_clients[i].GetFd());
    }
    if(_server_socketFd != -1)
    {
        std::cout << "Closing the Server Socket FD" << std::endl;
        close(_server_socketFd);
    }
}
void Server::ServerSocket()
{
    _server_socketFd = socket(AF_INET, SOCK_STREAM, 0);
    if(_server_socketFd == -1)
    {
        throw(std::runtime_error("Failed to start"));
        close(_server_socketFd);
    }
    int opt = 1;
    // set SO_REUSEADDR option
    // Fixies any issues if the address is in use case. 
    if(setsockopt(_server_socketFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        // Error exception

        std::cerr << "Socket Failure" << std::endl;
        close(_server_socketFd);
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in add; // the struct that contians the information of the server 
    add.sin_family = AF_INET; // IPv4
    add.sin_addr.s_addr = INADDR_ANY; 
    add.sin_port = htons(this->_port);

    std::cout << "Server IP address: " << inet_ntoa(add.sin_addr) << std::endl;
    // binding the Ip & the port to the network

    if(bind(_server_socketFd, (struct sockaddr *)&add, sizeof(add)) < 0)
    {
         std::cerr << "Failed to Bind the socket";
         close(_server_socketFd);
    }
    listen(_server_socketFd, 3); //  I am setting the backlog to 3 to allow connection up to 3 to be in queue
    // The backlog is system dependant. for linux the maximium is 256 or we can use SOMACONN to be automatically decided by the system
    
    
    // Setting the socket for the non blocking

    // int flags = fcntl(_server_socketFd, F_GETFL, 0);
    // When you set a socket to non-blocking mode, fcntl changes the behavior of the socket functions (e.g., accept, recv, send). 
    //In non-blocking mode, these functions return immediately if an operation cannot be completed, instead of waiting (or "blocking").
    if(fcntl(_server_socketFd, F_SETFL, O_NONBLOCK) < 0)
    {
        throw(std::runtime_error("Failed to Set to Non-blocking"));
    }
    
    
 // Poll  : Poll is used to monitor multiple file descriptor. 
 // Uses an array of pull structures each representing a specific file descriptor.

 struct pollfd poll;
 
 poll.fd = _server_socketFd;
 poll.events = POLLIN; // for reading data.
 poll.revents = 0;
 _fds.push_back(poll);
}

void Server::ServerInt()
{
    this->_port = 3030;
    ServerSocket();
    std::cout << "Server " << _server_socketFd << " Connection Established" << std::endl;
    
    // Running the server until the signal is received from the client

    while (Server::_signal == false ) {
        int pol =   poll(&_fds[0], _fds.size(), -1);
        if(pol < 0 )
            throw (std::runtime_error("Poll failed")); // wait for an event.
        for(size_t i(0); i < _fds.size(); i++)
        {
            if(_fds[i].revents & POLLIN)
            {
                if(_fds[i].fd == _server_socketFd)
                    AcceptNewClient(); // Allow the connection for the new client
                else
                 ReceiveNewData(_fds[i].fd);
            }
        }
    
    }
    CloseFds();
}


// Accepting new Client

void Server::AcceptNewClient()
{
    Client client;
    struct sockaddr_in client_id;
    struct pollfd new_poll;
    socklen_t len = sizeof(client_id);

    int connect_fd = accept(_server_socketFd, (sockaddr *)&(client_id),  &len);
    if (connect_fd == -1) {
        std::cout << "Failed to Accept" << std::endl;
        return;
    
    }
    if(fcntl(connect_fd, F_SETFL, O_NONBLOCK) == -1)
    {
        std::cout << "Failed " << std::endl; 
        return;
    }

    new_poll.fd = connect_fd; // add the fd to the pool
    new_poll.events = POLLIN; // Setting for reading the data from the fd
    new_poll.revents = 0;
    
    client.SetFd(connect_fd);
    client.SetIpAddress(inet_ntoa(client_id.sin_addr)); // convert the IP address to the string 
    _clients.push_back(client);
    _fds.push_back(new_poll);  // Adding the client socket to the stack of poll fds


    // connected 

    std::cout << connect_fd <<  " Connected " << std::endl;
    
}


void Server::ReceiveNewData(int fd)
{
    char buff[1024]; // buffer for the received data
    memset(buff,0,sizeof(buff));
    
    ssize_t byte = recv(fd, buff, sizeof(buff)-1, 0);
    if(byte <= 0)
    {
        std::cout << "Client Disconnected" << std::endl;
        ClearClients(fd);
        close(fd);
    }
    else 
    {
        buff[byte] = '\0';
        std::cout << "....Clients Data...." << std::endl;
        std::cout << buff << std::endl;

        // We can parse, check, authenticate, handle the command, etc
    }
}
int main(void)
{
    Server ser;
    std::cout << "--------Server ----------------" << std::endl;
    try
    {
        signal(SIGINT, Server::SignalHandler);
        ser.ServerInt();
        // ser.ServerSocket();
        
    }catch(const std::exception &err)
    {
        ser.CloseFds();
        std::cerr << err.what() << std::endl; 
    }
}