/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Privmsg.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamone <pamone@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 23:50:12 by pamone            #+#    #+#             */
/*   Updated: 2024/11/23 23:50:13 by pamone           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Command.hpp"

void	Command::Privmsg_cmd( void ) 
{
	
	Client		*receive;
	Channel		*channel;
	std::string	msg;
	std::string	response;

	if (this->_args.size() == 0)
		return (sendMessage(RED "Error: A nick must be provided!" RESET, "411"));
	if (this->_args.size() == 1)
		return (sendMessage(RED "Error: A message must be provided!" RESET, "412"));
	msg = Extra::joinSplitItr(this->_args.begin() + 1, this->_args.end());
	std::vector<std::string>	pmu;
	pmu = Extra::split(this->_args[0], ',');
	for (size_t i = 0; i < pmu.size(); i++)
	{
		if (pmu[i][0] != '#') 
		{
			receive = this->_ircServer.getClientByNick(pmu[i]);
			if (receive == NULL)
				{
					sendMessage(RED "Error: Nick not found!" RESET, "401");
					continue;
				}
			if (msg[0] == ':')
				msg.erase(0, 1);
			response = ":" + this->userN.getNickN() + " Send PRIVMSG To " + receive->getNickN() + " :" + msg;
			receive->recivMessage(response);
		}
		else 
		{
			channel = this->_ircServer.getChannelByName(pmu[i]);
			if (channel == NULL)
			{
				sendMessage(RED "Error: Channel Not Found!" RESET, "403");
				continue;
			}
			if (channel->getClientByNick(this->userN.getNickN()) == NULL)
			{
				sendMessage(RED "Error: you are not in Channel!" RESET, "442");
				continue;
			}
			if (msg[0] == ':')
				msg.erase(0, 1);
			response = ":" + this->userN.getNickN() + " Send PRIVMSG To " + channel->getChannelName() + " :" + msg;
			channel->messageToChannel(response, this->userN.getFd());
		}
	}
	return ;
}
