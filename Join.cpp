/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamone <pamone@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 23:51:37 by pamone            #+#    #+#             */
/*   Updated: 2024/11/23 23:51:38 by pamone           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

void	Command::Join_cmd( void ) 
{
	Channel		*channel;
	std::string	users;
	std::string	channelName;
	std::string	password;
	
	if (this->_args.size() < 1 || this->_args.size() > 2)
		return (sendMessage(RED "Error: usage: /JOIN <channels> [<keys>]" RESET, "461"));
	password = this->_args.size() == 1 ? "" : this->_args[1];
	std::vector<std::string>	cs;
	cs = Extra::split(this->_args[0], ',');

	for (size_t i = 0; i < cs.size(); i++)
	{
		if (cs[i][0] != '#')
			cs[i] = "#" + cs[i];
		channel = this->_ircServer.getChannelByName(cs[i]);
		if (channel == NULL) 
		{
			channel = new Channel(cs[i], password);
			this->_ircServer.addChannel(channel);
			channel->addOper(&this->userN);
		}
		else
		{
			if (channel->check_mode('i'))
				return (sendMessage(RED "Error: channel mode is invite only" RESET, "473"));
		}
		if (channel->getClientByNick(this->userN.getNickN()) != NULL)
			return (sendMessage(RED "Error: You already are in this channel!" RESET, "443"));
		if (channel->check_mode('l'))
		{
			if (channel->getUsersname().size() >= channel->getLimit())
				return (sendMessage(RED "Error: channel is full!" RESET, "471"));
		}
		if (channel->check_mode('k')) //will check the password only if the mode was set to k
		{
			if (password == channel->getPassword())
				this->userN.addChannel(channel);
			else
				return (sendMessage(RED "Error: Password incorrect to channel!" RESET, "475"));
		}
		else
			this->userN.addChannel(channel);
		channel->messageFrChannel(":" + this->userN.getNickN() + " JOIN " + channel->getChannelName());
		users = Extra::joinSplit(channel->getUsersname());
		channelName = channel->getChannelName();
		if (channelName[0] == '#')
			channelName.erase(0, 1);
		sendMessage(users, "353", 0, "= " + channelName + ", include these Nicknames :");
	
	}
	return ;
}