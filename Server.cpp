/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schennal <schennal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 12:31:50 by pamone            #+#    #+#             */
/*   Updated: 2024/11/06 20:49:45 by schennal         ###   ########.fr       */
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

bool isPortValid(int port) {
    // Check if port is in the valid range (1024 - 65535)
    return (port >= 1024 && port <= 65535);
}

bool isPortAvailable(int port) {
    // Create a socket for checking port availability
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::cerr << "Error creating socket." << std::endl;
        return false;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    // Try to bind the socket to the port
    int bind_result = bind(sockfd, (struct sockaddr*)&addr, sizeof(addr));
    close(sockfd);

    // If bind failed, the port is already in use
    return bind_result == 0;  // bind_result == 0 means the port is available
}

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
         exit(EXIT_FAILURE);
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

void Server::ServerInt(int port, const std::string& password)
{
    if (!isPortValid(port)) {
        throw std::invalid_argument("Invalid port number. Port must be between 1024 and 65535.");
    }

    if (!isPortAvailable(port)) {
        throw std::runtime_error("Port is already in use or unavailable.");
    }

    
    this->_port = port;
    this->_password = password;
    ServerSocket();
    std::cout << "Server listening on port " << _port << std::endl;
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


Client* Server::FindClientByFd(int fd) {
    // Loop through all clients to find the one with the matching file descriptor
    for (size_t i = 0; i < _clients.size(); i++) {
        Client& client = _clients[i];  // Explicitly declare the type of 'client'
        if (client.GetFd() == fd) {
            // Client found, return the pointer to the client
            return &client;
        }
    }
    return NULL;  // Return NULL if no client with the given fd is found
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


std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    size_t last = str.find_last_not_of(" \t\r\n");
    if (first == std::string::npos || last == std::string::npos)
        return "";
    return str.substr(first, (last - first + 1));
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
        std::cout << "....Clients Data...." << buff << std::endl;
        
        
        Client *client = FindClientByFd(fd);
        // Find client by file descriptor
        // for (size_t i = 0; i < _clients.size(); i++) {
        //     if (_clients[i].GetFd() == fd) {
        //         client = &_clients[i];
        //         break;
        //     }
        // }

        if (client)
        {
            if (!client->IsAuthenticated()) {

                std::cout << "Server stored password: '" << _password << "'" << std::endl;  // Print server-side password

               
                // If not authenticated, check for password
                if (std::string(trim(buff)) == _password) {
                    client->SetAuthenticated(true);
                    send(fd, "Authentication successful!\n", 27, 0);
                } else {
                    send(fd, "Incorrect password. Disconnecting...\n", 38, 0);
                    ClearClients(fd);
                    close(fd);
                }
            }
            else {
                // Handle normal commands (nickname, join channel, etc.)
                HandleCommand(fd, buff); // Implement command handling here
            }

        // We can parse, check, authenticate, handle the command, etc
        }
    }
}


Channel* Server::FindOrCreateChannel(const std::string& channel_name) {
    // Loop through all existing channels
    for (size_t i = 0; i < _channels.size(); i++) {
        Channel& channel = _channels[i];  // Explicitly declare the type of 'channel'
        if (channel.name == channel_name) {
            // Channel found, return it
            return &channel;
        }
    }

    // If the channel doesn't exist, create a new one
    _channels.push_back(Channel(channel_name));
    return &_channels.back();  // Return the newly created channel
}






