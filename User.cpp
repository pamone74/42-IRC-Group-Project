#include "Command.hpp"


void	Command::User_cmd( void ) 
{
	if (this->_args.size() != 4)
		return (sendMessage(RED "Error: usage: /USER <realname> <username> <servername> <hostname>" RESET, "461"));
	if (this->userN.getUserN().empty() == false)
		return (sendMessage(RED "Error: are already register!" RESET, "462"));
	this->userN.setRealN(this->_args[0]);
	this->userN.setUserN(this->_args[1]);
	this->userN.setServerN(this->_args[2]);
	this->userN.setHostN(this->_args[3]);
	sendMessage("Welcome to the ft_irc server " +
		this->userN.getNickN() + "! " + this->userN.getUserN() + "@" + this->userN.getServerN() + this->userN.getHostN(), "001");
	if (this->_ircServer.checkOperators() == false){
		sendMessage(PINK "You are now an IRC operator!" RESET, "381", this->userN.getFd());
			this->_ircServer.messageToServer(":127.0.0.1 001 all :" + this->userN.getNickN() + " is an operator now!", this->userN.getFd());
		this->userN.setOper();
	}
	sendMessage(GREEN "Available commands:" RESET, "001", this->userN.getFd());
	sendMessage(GREEN "+----------+-----------------------------------------------+" RESET, "001", this->userN.getFd());
	sendMessage(GREEN "| Command  | Description                                   |" RESET, "001", this->userN.getFd());
	sendMessage(GREEN "+----------+-----------------------------------------------+" RESET, "001", this->userN.getFd());
	sendMessage(PINK "| /QUIT    | <reason>                                      |" RESET, "001", this->userN.getFd());
	sendMessage(PINK "| /USER    | <realname> <username> <servername> <hostname> |" RESET, "001", this->userN.getFd());
	sendMessage(PINK "| /OPER    | <nick> <password>                             |" RESET, "001", this->userN.getFd());
	sendMessage(PINK "| /KILL    | <nick> <comment>                              |" RESET, "001", this->userN.getFd());
	sendMessage(BLUE "| /PRIVMSG | <nick> <msg!>                                 |" RESET, "001", this->userN.getFd());
	sendMessage(CYAN "| /PART    | <channel>                                     |" RESET, "001", this->userN.getFd());
	sendMessage(CYAN "| /JOIN    | <channels> [<keys>]                           |" RESET, "001", this->userN.getFd());
	sendMessage(CYAN "| /INVITE  | <nick> <channel>                              |" RESET, "001", this->userN.getFd());
	sendMessage(CYAN "| /MODE    | <channel> [+|-]modechars [parameters]         |" RESET, "001", this->userN.getFd());
	sendMessage(CYAN "| /TOPIC   | <channel> [<new topic>]                       |" RESET, "001", this->userN.getFd());
	sendMessage(CYAN "| /KICK    | <channel> <nick> [<comment>]                  |" RESET, "001", this->userN.getFd());
	sendMessage(GREEN "+---------+------------------------------------------------+" RESET, "001", this->userN.getFd());

	return ;
}