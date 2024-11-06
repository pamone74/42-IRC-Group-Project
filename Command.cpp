/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schennal <schennal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 18:54:40 by schennal          #+#    #+#             */
/*   Updated: 2024/11/06 19:06:49 by schennal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Server.hpp"

#include "Channel.hpp"

void Server::HandleJoinCommand(Client* client, const std::string& channel_name) {
    // Check if channel exists
    Channel* channel = FindOrCreateChannel(channel_name);
    channel->AddUser(client);
    send(client->GetFd(), ("You joined the channel " + channel_name + "\n").c_str(), 32, 0);
}