/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schennal <schennal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 00:51:30 by pamone            #+#    #+#             */
/*   Updated: 2024/11/06 20:49:36 by schennal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef SERVER_HPP
#define SERVER_HPP


#include <iostream>
#include <vector> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <netinet/in.h> //-> for sockaddr_in
#include <fcntl.h> //-> for fcntl()
#include <unistd.h> //-> for close()
#include <arpa/inet.h> //-> for inet_ntoa()
#include <poll.h> //-> for poll()
#include <csignal> //-> for signal()

#include "Client.hpp"
#include "Channel.hpp"



class Server{

    private:
        int _port;
        int _server_socketFd;
        static bool _signal;
        std::vector<Client> _clients;
        std::vector<struct pollfd> _fds;
        std::vector<Channel> _channels; 

        std::string _password;

    public:
        Server(){_server_socketFd = -1;};  // default constructor
        Server(const std::string& password) : _password(password) {}
        void ServerInt(int port, const std::string& password); 
        void ServerSocket();
        void AcceptNewClient();
        void ReceiveNewData(int fd);
        static void SignalHandler(int signum);
        void CloseFds();
        void ClearClients(int fd)
        {
            close(fd);
        }; // Clear clients
        

        // Exception class
        // class Error : public std::exception
        // {
        //     public:
        //         Error(std::string &message):_message(message){};
        //         const char* what() const noexcept override {
        //             return _message.c_str();
        //         }

        //     private:
        //         std::string _message;
        // }



        void HandleJoinCommand(Client* client, const std::string& channel_name);
        void HandleCommand(int fd, const std::string& command);
   
        Client* FindClientByFd(int fd);  

        void AuthenticateClient(int fd, std::string cmd);

        Channel* FindOrCreateChannel(const std::string& channel_name);
};

#endif