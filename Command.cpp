/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamone <pamone@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 23:53:34 by pamone            #+#    #+#             */
/*   Updated: 2024/11/23 23:53:35 by pamone           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

//constructor (Buffer received of the user, /userFd Socket fd from user,ircServer Instance of server )
Command::Command( std::string buffer, int clientFd, Server & ircServer ): userN(*ircServer.getClientByFd(clientFd)), _ircServer(ircServer)
 {
	parserBuffer(buffer);
	checkCommand();
	return ;
}

Command::~Command( void )
 {
	return ;
 }

//parse the buffer received
//the first arg will be command, the rest will be attributes of the command
void	Command::parserBuffer( std::string buffer ) {
	if (buffer[0] == '/') //to remove / if found in the begining of the command
		buffer.erase(0, 1);
	buffer.erase(std::remove(buffer.begin(), buffer.end(), '\n'), buffer.end());
	buffer.erase(std::remove(buffer.begin(), buffer.end(), '\r'), buffer.end());
	this->_args = Extra::split(buffer, ' ');
	if (this->_args.size() == 0)
		return ;
	this->_command = Extra::toupper(*this->_args.begin());// make command in CAPS letters
	this->_args.erase(this->_args.begin());
	if (this->_args[0][0] == ':')
		this->_args[0].erase(0, 1);
	return ;
}

//check if command is accepted
void	Command::checkCommand( void )
{
	if (this->_command == "PASS")
		Pass_cmd();
	else if (this->_command == "QUIT")
		Quit_cmd();
	else if (this->_command == "HELP")
		Help_cmd();
	else if (this->_command == "PONG")
		return ;
	else if (this->userN.isAuthntec()){
		if (this->_command == "NICK")
			Nick_cmd();
		else if (this->userN.getNickN().empty() == false) {
			if (this->_command == "USER")
				User_cmd();
			else if (this->userN.getUserN().empty() == false) {
				if (this->_command == "PRIVMSG")
					Privmsg_cmd();
				else if (this->_command == "JOIN")
					Join_cmd();
				else if (this->_command == "OPER")
					Oper_cmd();
				else if (this->_command == "PART")
					Part_cmd();
				else if (this->_command == "KICK")
					Kick_cmd();
				else if (this->_command == "MODE")
					Mode_cmd();
				else if (this->_command == "KILL")
					Kill_cmd();
				else if (this->_command == "TOPIC")
					Topic_cmd();
				else if (this->_command == "INVITE")
					Invite_cmd();
			}
			else
				return (sendMessage(RED "Error: A user must be provide: usage: /USER <username> <hostname> <servername> <realname>" RESET, "431"));
		}
		else
			return (sendMessage(RED "Error: A nick must be provide: usage: /NICK <nick>" RESET, "431"));
	} else
		return (sendMessage(RED "Error: Password is required: usage: /PASS <password>" RESET, "464"));
	return ;
}

//function to send numeric response either a reply or error
void	Command::sendMessage( std::string msg, std::string code, int fd, std::string opt ) {
	std::string	response;
	std::string	nick = this->userN.getUserN();

	if (nick.empty())
		nick = "Unknown";
	response = ":127.0.0.1 " + code + " " + nick + " ";
	if (opt != "")
		response += opt + " ";
	response += msg + "\r\n";
	if (fd == 0)
		fd = this->userN.getFd();
	if (send(fd, response.c_str(), strlen(response.c_str()), 0) < 0)
		std::cerr << "sendMessage: send:" << strerror(errno);
	return ;
}

bool Command::is_valid_mode(char c)
{
	std::string mode = "itkol";

	for (int i = 0; i < (int)mode.size(); i++)
	{
		if (c == mode[i])
			return (true);
	}
	return (false);
}