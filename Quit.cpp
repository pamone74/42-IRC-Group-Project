/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamone <pamone@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 23:50:12 by pamone            #+#    #+#             */
/*   Updated: 2024/11/23 23:50:13 by pamone           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

void	Command::Quit_cmd( void ) 
{
		Client		*user;
	std::string	response;

	if (this->_args.size() < 1)
		return (sendMessage(RED "Error: usage: /QUIT <reason>" RESET, "461"));
	response = ":" + this->userN.getNickN() + " QUIT :" + Extra::joinSplitItr(this->_args.begin(), this->_args.end());
	this->_ircServer.messageAllclients(response);
	std::cout << RED << "User left: " << this->userN.getFd() << RESET << std::endl;
	if(this->userN.isOper())
		this->userN.setOper();
	if (this->_ircServer.getClients().size() != 1	&& this->_ircServer.checkOperators() == false) {
		user = *(this->_ircServer.getClients().begin() + 1);
		if (user != NULL) {
			user->isOper();
			this->_ircServer.messageToServer(":127.0.0.1 001 all :" + user->getNickN() + " is an operator now!", user->getFd());
			sendMessage(GREEN + user->getNickN() + " successfully You are now an IRC operator!" RESET, "381", user->getFd());
		}
	}
	this->_ircServer.deleteClient(this->userN.getFd());
	return ;
}