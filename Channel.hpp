/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schennal <schennal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 18:48:36 by schennal          #+#    #+#             */
/*   Updated: 2024/11/06 19:50:36 by schennal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP


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



class Channel {
public:
    std::string name;
    std::vector<Client*> users;
    // Other channel features like topic, mode, etc.

    Channel(const std::string& channel_name) : name(channel_name) {}
    void AddUser(Client* client) { users.push_back(client); }
    void RemoveUser(Client* client) { users.erase(std::remove(users.begin(), users.end(), client), users.end()); }
};

#endif