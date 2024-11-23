/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamone <pamone@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 23:54:08 by pamone            #+#    #+#             */
/*   Updated: 2024/11/23 23:54:09 by pamone           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp" 

Client::Client(int fduser):fd_user(fduser),nickN(""),userN(""),authent(false),oper(false)
{
    return;
}

void Client::Authn(void)
{
    this->authent = true;
}

void Client::recivMessage(std::string msg)
{
    if(msg.find("\r\n")==std::string::npos)
         msg = msg + "\r\n";
    if (send(getFd(), msg.c_str(), strlen(msg.c_str()), 0) < 0)
		std::cerr << "receiveMessage: send: " << strerror(errno) << std::endl;
}

bool Client::isAuthntec(void)
{
    return(this->authent);
}

void Client::setNickN(std::string nick)
{ 
    this->nickN = nick;
}

void Client::setUserN(std::string username)
{
    this->userN = username;
}

void Client::setRealN(std::string realname)
{
    this->realN = realname;
}

void Client::setHostN(std::string hostname)
{
    this->hostN = hostname;
}

void Client::setServerN(std::string servername)
{
    this->serverN = servername;
}

void Client::addChannel(Channel *channel)
{
    std::vector<Channel *>::iterator	i = this->chanVec.begin();
	for ( ; i != this->chanVec.end(); i++)
		if ((*i)->getChannelName() == channel->getChannelName())
			return ;
	this->chanVec.push_back(channel);
	channel->addUser(this);
	return ;
}

int Client::getFd(void)
{
    return (this->fd_user);
}

std::string Client::getNickN(void)
{
    return (this->nickN);
}

std::string Client::getUserN(void)
{
    return (this->userN);
}

std::string Client::getRealN(void)
{
    return (this->realN);
}

std::string Client::getHostN(void)
{
    return (this->hostN);
}

std::string Client::getServerN(void)
{
    return (this->serverN);
}

std::vector<Channel *> Client::getChannels(void)
{
    return std::vector<Channel *>();
}

void Client::rmChannel(Channel *channel)
{
    std::vector<Channel *>::iterator	i = this->chanVec.begin();
	for ( ; i != this->chanVec.end(); i++)
     {
		if (*i == channel) 
        {
			this->chanVec.erase(i);
			return ;
		}
	}
	return ;
}

void Client::setOper(void)
{
    this->oper = ! this->oper;
}

bool Client::isOper(void)
{
    return(this->oper);
}

Client::~Client(void)
{
    close(this->fd_user);
    return;
}
