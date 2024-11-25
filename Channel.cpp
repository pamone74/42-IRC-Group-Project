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

// Parameterized constructor to initialize the channel name and password.
Channel::Channel(std::string name, std::string password):channel_name(name),passworD(password)
{
    return;
}

// Adds a user to the channel's list of users if the user is not already in the channel.
void Channel::addUser(Client *user)//this function used to add new user
{
    if(getClientByNick(user->getNickN()) == NULL)
    this->usrs_name.push_back(user);
    return;
}

// Removes a user from the channel's list of users if found.
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

// Iterates over all users in the channel and sends the given message to each user.
// Ensures the message ends with '\r\n' for proper formatting.
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

// Sends a message to all users in the channel except the one identified by senderFd.
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

// Checks if the channel has a specific mode enabled.
bool Channel::check_mode(char c)
{
    for(size_t i =0; i <modE.size(); i++)
    {
        if(modE[i] == c)
        return(true);
    }
    return(false);
}


// Adds or removes a user to/from the list of channel operators.
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


// Updates channel properties like topic, mode, password, or user limit.
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


// Provides access to channel properties like limit, name, password, users, topic, and mode.
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


// Searches for a user in the list of users or operators by their nickname.
Client *Channel::getClientByNick(std::string nick)
{
    
    std::vector<Client *>::iterator     i = this->usrs_name.begin();
    if(nick[0] == ':')
    nick.erase(0,1);
    for (; i != this->usrs_name.end(); i++)
    {
        if((*i)->getNickN()== nick)
        return(*i);
    }
    return(NULL);
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
