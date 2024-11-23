/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamone <pamone@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 23:51:34 by pamone            #+#    #+#             */
/*   Updated: 2024/11/23 23:51:35 by pamone           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

void	Command::Kick_cmd( void ) 
{
	Channel		*channel;
	Client		*user;
	std::string	msg;
	std::string	response;
	
	if (this->_args[0] == this->userN.getNickN())
		this->_args.erase(this->_args.begin());
	if (this->_args.size() < 2)
		return (sendMessage(RED "Error: usage: /KICK <channel> <user> [<comment>]" RESET, "461"));
	if (this->_args[0][0] != '#')
		this->_args[0] = '#' + this->_args[0];
	channel = this->_ircServer.getChannelByName(this->_args[0]);
	if (channel == NULL)
		return (sendMessage(RED "Error: Channel Not Found!" RESET, "403", 0, this->_args[0]));
	if (channel->getOperByNick(this->userN.getNickN()) == false)
		return (sendMessage(RED "Error: You need be operator to kick a user!" RESET, "482", 0, channel->getChannelName()));
	if (channel->getClientByNick(this->userN.getNickN()) == NULL)
		return (sendMessage(RED "Error: You need be a member of the channel to kick a user!" RESET, "442", 0, channel->getChannelName()));
	user = channel->getClientByNick(this->_args[1]);
	if (user == NULL)
		return (sendMessage(RED "Error: This user isn't in this channel!" RESET, "441", 0, channel->getChannelName() + " " + this->_args[1]));
	msg = Extra::joinSplitItr(this->_args.begin() + 2, this->_args.end());
	if (msg[0] == ':')
		msg.erase(0, 1);
	response = ":" + this->userN.getNickN() + " KICK " + channel->getChannelName() + " " + user->getNickN() + " " + msg;
	user->rmChannel(channel);
	user->recivMessage(response);
	channel->removeUser(user);
	channel->messageFrChannel(response);
	return ;
}