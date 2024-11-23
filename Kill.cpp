/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kill.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamone <pamone@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 23:51:28 by pamone            #+#    #+#             */
/*   Updated: 2024/11/23 23:51:30 by pamone           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

void	Command::Kill_cmd( void ) 
{
	Client		*user;
	std::string	msg;
	std::string	response;

	if (this->_args.size() < 2)
	 	return (sendMessage(RED "Error: usage: /KILL <nick> <comment>" RESET, "461"));
	if (this->userN.isOper() == false)
		return (sendMessage(RED "Error: You need be server operator to kill!" RESET, "485", this->userN.getFd()));
	user = this->_ircServer.getClientByNick(this->_args[0]);
	if (user == NULL)
		return (sendMessage(RED "Error: The user passed doesn't exist!" RESET, "401"));
	msg = Extra::joinSplitItr(this->_args.begin() + 1, this->_args.end());
	if (msg[0] == ':')
	 	msg.erase(0, 1);
	response = ":" + this->userN.getNickN() + " KILL " + user->getNickN() + " . " + msg;
	user->recivMessage(response);
	response = user->getNickN() + " have been killed by " + this->userN.getNickN();
	this->_ircServer.messageToServer(response, user->getFd());
	std::cout << "User left: "<< user->getFd() << std::endl;
	this->_ircServer.deleteClient(user->getFd());
	return ;
}