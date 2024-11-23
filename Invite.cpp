/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamone <pamone@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 23:52:47 by pamone            #+#    #+#             */
/*   Updated: 2024/11/23 23:52:48 by pamone           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

void	Command::Invite_cmd( void ) 
{
	Channel *channel;
	std::string	users;
	
	if (this->_args.size() !=2)
	{
		return (sendMessage(RED "Error: usage: /INVITE <NICK> <channel>" RESET, "461"));
	}
	std::string Username = this->_args[0];
	std::string Channelname = this->_args[1];
	Client *invited=this->_ircServer.getClientByNick(Username);
	if(!invited)//if there is no user invited, there is no user in channel
	{
		return (sendMessage(RED "Error: NO USER " RESET, "444"));
	}
	if(!this->_ircServer.getChannelByName(Channelname))
	{//if ther is no hannel created yet 
		return (sendMessage(RED "Error: NO Channel " RESET, "403"));
	}
	else 
		channel= this->_ircServer.getChannelByName(Channelname);
	if (channel->getOperByNick(this->userN.getNickN()) == false)
	{
		return (sendMessage(RED "Error: You need be channel operator to invite a user!" RESET, "482", 0, Channelname));
	}
	//std::cout <<"in" <<std::endl;
	//std::cout <<"userbynick: "<< Username <<std::endl;
	if ((this->_ircServer.getChannelByName(Channelname))->getClientByNick(Username) != NULL)
		return (sendMessage(RED "Error: User Already are in this channel!" RESET, "443")); //y
	else 
	{
		//std::cout <<"the user is : "<< invited->getNick() <<std::endl;
		invited->addChannel(this->_ircServer.getChannelByName(Channelname));
		(this->_ircServer.getChannelByName(Channelname))->messageFrChannel(":" + invited->getNickN() + " JOIN " + channel->getChannelName());
		users = Extra::joinSplit((this->_ircServer.getChannelByName(Channelname))->getUsersname());
		sendMessage(users, "353", 0, "= " + Channelname + ", include these Nicknames :");
		sendMessage("End of /NAMES list", "366", 0, Channelname);
	}
}
