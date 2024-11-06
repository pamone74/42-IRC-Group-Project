/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schennal <schennal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 18:55:19 by schennal          #+#    #+#             */
/*   Updated: 2024/11/06 19:06:43 by schennal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Server.hpp"
#include "Channel.hpp"



void Server::HandleCommand(int fd, const std::string& command) {
    if (command.find("NICK") == 0) {
        // Set the client's nickname
        std::string nickname = command.substr(5);
        Client *client = FindClientByFd(fd);
        if (client) {
            client->SetNickname(nickname);
            send(fd, ("Your nickname is set to " + nickname + "\n").c_str(), 32, 0);
        }
    }
    else if (command.find("USER") == 0) {
        // Set the client's username
        std::string username = command.substr(5);
        Client *client = FindClientByFd(fd);
        if (client) {
            client->SetUsername(username);
            send(fd, ("Your username is set to " + username + "\n").c_str(), 32, 0);
        }
    }
}