/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamone <pamone@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 23:51:05 by pamone            #+#    #+#             */
/*   Updated: 2024/11/23 23:51:06 by pamone           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

void	Command::Part_cmd( void ) {
	Channel								*channel;
	std::string							response;
	std::vector<std::string>::iterator	it;

	if (this->_args.size() < 1)
		return (sendMessage(RED "Error: usage: /PART <channel>" RESET, "461"));
	it = this->_args.begin();
	for ( ; it != this->_args.end(); it++) 
	{
		if ((*it)[0] == ':')
			it->erase(0, 1);
		if ((*it)[0] != '#')
			*it = '#' + *it;
		channel = this->_ircServer.getChannelByName(*it);
		if (channel == NULL)
			sendMessage(RED "Error: There is no channel with this name!" RESET, "403", 0, *it);
		else if (channel->getClientByNick(this->userN.getNickN()) == NULL)
			sendMessage(RED "Error: You aren't in this channel!" RESET, "442", 0, *it);
		else {
			response = ":" + this->userN.getNickN() + " PART " + *it;
			channel->messageFrChannel(response);
			this->userN.rmChannel(channel);
			channel->removeUser(&this->userN);
		}
	}
	return ;
}