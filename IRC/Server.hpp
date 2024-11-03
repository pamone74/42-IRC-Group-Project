/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamone <pamone@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 00:51:30 by pamone            #+#    #+#             */
/*   Updated: 2024/11/03 01:58:24 by pamone           ###   ########.fr       */
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

class Server{

    private:
        int _port;
        int _server_socketFd;
        static bool _signal;
        std::vector<Client> _clients;
        std::vector<struct pollfd> _fds;

    public:
        Server(){_server_socketFd = -1;}; // default constructor
        void ServerInt(); 
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
};

#endif