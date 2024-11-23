/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Oper.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamone <pamone@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 23:51:08 by pamone            #+#    #+#             */
/*   Updated: 2024/11/23 23:51:09 by pamone           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

void	Command::Oper_cmd( void )
 {
	Client	*user;

	if (this->_args.size() != 2)
		return (sendMessage(RED "Error: usage: /OPER <nick> <password>" RESET, "461"));
	user = this->_ircServer.getClientByNick(this->_args[0]);
	if (user == NULL)
		return (sendMessage(RED "Error: The nickName passed doesn't found!" RESET, "401"));
	if (user->isOper())
		return (sendMessage(RED "Error: The nickName is an operator already!" RESET, "610"));
	if (this->_args[1] == OPERATOR_PASS)
		user->setOper();
	else
		return (sendMessage(RED "Error: Incorrect Password!" RESET, "464"));
	this->_ircServer.messageToServer(":127.0.0.1 001 all :" + this->_args[0] + " is an operator now!", user->getFd());
	sendMessage(this->_args[0] + " You are now an IRC operator!", "381", user->getFd());
	return ;
}