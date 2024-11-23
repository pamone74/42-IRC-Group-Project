/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamone <pamone@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 23:54:16 by pamone            #+#    #+#             */
/*   Updated: 2024/11/23 23:54:17 by pamone           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "irc.hpp"

Channel::Channel(std::string name, std::string password):channel_name(name),passworD(password)
{
    return;
}//perameter constructor

void Channel::addUser(Client *user)//this function used to add new user
{
    if(getClientByNick(user->getNickN()) == NULL)
    this->usrs_name.push_back(user);
    return;
}

void Channel::removeUser(Client *user)
{
    std::vector<Client *>::iterator	i = this->usrs_name.begin();
	for ( ; i != this->usrs_name.end(); i ++)
     {
		if (*i == user)
         {
			this->usrs_name.erase(i);
			return ;
		}
	}
	return ;
}

void Channel::messageFrChannel(std::string msg)
{
    std::vector<Client *>::iterator	i = this->usrs_name.begin();//iterate over the list of users 
	if (msg.find("\r\n") == std::string::npos)
		msg = msg + "\r\n";
	for( ; i != this->usrs_name.end(); i++)
		if (send((*i)->getFd(), msg.c_str(), strlen(msg.c_str()), 0) < 0)
              std::cerr << "message From Channel: send: " << strerror(errno) << std::endl;
            return;
	
}

void Channel::messageToChannel(std::string msg, int senderFd)
{
    std::vector<Client *>::iterator	i = this->usrs_name.begin();
	if (msg.find("\r\n") == std::string::npos)
		msg = msg + "\r\n";
	for ( ; i != this->usrs_name.end(); i++)
		if ((*i)->getFd() != senderFd)
			if (send((*i)->getFd(), msg.c_str(), strlen(msg.c_str()), 0) < 0)
                std::cerr << "message To Channel: send:" << strerror(errno) << std::endl;
	return ;
}

bool Channel::check_mode(char c)
{
    for(size_t i =0; i <modE.size(); i++)
    {
        if(modE[i] == c)
        return(true);
    }
    return(false);
}

void Channel::removeOper(Client *user)
{
    std::vector<Client *>::iterator i = this->operets.begin();
    for(; i != this->operets.end(); i++)
    {
    if(*i == user)
    {
        this->operets.erase(i);
        return;
    }
    }
    return;

}

void Channel::addOper(Client *user)// add new operators
{
    if(getOperByNick(user->getNickN()) == NULL)
    this->operets.push_back(user);
    return;
}

void Channel::setTopic(std::string topic)
{
    this->topiC= topic;
}

void Channel::setMode(std::string mode)
{
    this->modE = mode;
}

void Channel::setPassword(std::string pass)
{
    this->passworD = pass;
}

void Channel::setLimit(unsigned int limit)
{ 
    this->limiT = limit;
}

unsigned int Channel::getLimit()
{
    return (limiT);
}

std::string Channel::getChannelName(void)
{
    return (this->channel_name);
}

std::string Channel::getPassword(void)
{
    return (this->passworD);
}

std::vector<Client *> Channel::getUsersname(void)
{
    return (this->usrs_name);
}

std::string Channel::getTopic()
{
    return (topiC);
}

std::string Channel::getMode()
{
    return (modE);
}

Client *Channel::getClientByNick(std::string nick)
{
    // we create vector itroters to itreate through list of users
    std::vector<Client *>::iterator     i = this->usrs_name.begin();
    if(nick[0] == ':')// if the nick is :wadha, first array is :
    nick.erase(0,1); //start erase from postion 0, how many char to erase is 1
    for (; i != this->usrs_name.end(); i++)//check if the itretors dosent reach to end
    {
        if((*i)->getNickN()== nick)//if i have nicks wadha sara mera , and im searching for sara
        return(*i);//if wadha == wadha return wadha
    }
    return(NULL);// if the nick name not found return null
}

Client *Channel::getOperByNick(std::string nick)
{
    std::vector<Client *>::iterator i = this->operets.begin();
    if (nick[0] == ':')
		nick.erase(0, 1);
	for ( ; i != this->operets.end(); i++) {
		if ((*i)->getNickN() == nick)
			return (*i);
	}
	return (NULL);

    
}

Channel::~Channel(void)
{
    return; 
}//destrctor
