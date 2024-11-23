/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamone <pamone@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 23:51:15 by pamone            #+#    #+#             */
/*   Updated: 2024/11/23 23:51:16 by pamone           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

void	Command::Nick_cmd( void ) 
{
	std::vector<Client *>			usersVec;
	std::vector<Client *>::iterator	it;

	if (this->_args.size() != 1)
		return (sendMessage(RED "Error: usage: /NICK <nick>" RESET, "431"));
	else if (this->_args[0].empty())
		return (sendMessage(RED "Error: Nick cannot be empty!" RESET, "432"));
	else if (this->_args[0] == "anonymous")
		return (sendMessage(RED "Error: This nick can't be used!" RESET, "432"));
	else if (Extra::invalidChar(this->_args[0]))
		return (sendMessage(RED "Error: This nick contain invalids characters!" RESET, "432"));
	usersVec = this->_ircServer.getClients();
	it = usersVec.begin();
	for( ; it != usersVec.end(); it++) 
	{
		if (this->_args[0] == (*it)->getNickN())
			return (sendMessage(RED "Error: This nick is already in use!" RESET, "432"));
	}
	this->_ircServer.messageAllclients(this->userN.getNickN() + ": change nick to " + this->_args[0]);
	this->userN.setNickN(this->_args[0]);
	return (sendMessage(GREEN "successfully this nick changed" RESET, "001"));
}