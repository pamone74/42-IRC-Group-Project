/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Help.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamone <pamone@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 23:52:51 by pamone            #+#    #+#             */
/*   Updated: 2024/11/23 23:52:52 by pamone           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

void	Command::Help_cmd( void )
 {
	sendMessage("START of /HELP list", "704");
	sendMessage("Commands Avaliable:", "705", this->userN.getFd());
	sendMessage("PASS, QUIT, HELP, NICK, USER,PRIVMSG, JOIN, OPER, PART, KICK, KILL", "705", this->userN.getFd());
	sendMessage("To see how to use a command, send: /<command> ", "705", this->userN.getFd());
	sendMessage("END of /HELP list", "706");
	return ;
}