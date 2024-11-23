/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamone <pamone@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 23:50:01 by pamone            #+#    #+#             */
/*   Updated: 2024/11/23 23:50:02 by pamone           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

void	Command::Topic_cmd(void)
{
	Channel		*cchannel;
	std::string channelName;
	std::string channelTopic;

	if (this->_args.size() < 1 || this->_args.size() > 2)
			return (sendMessage(RED "Error: usage: /TOPIC <channel> [<topicname>]" RESET, "461"));
	channelName = this->_args[0];
	if(!(cchannel = this->_ircServer.getChannelByName(channelName)))
		return (sendMessage(RED "Error: NO such channel " RESET, "403"));
	if (this->_args.size() == 1) //print the topic
	{
		sendMessage("The topic of " + channelName + " is " + cchannel->getTopic(), "4");
	}
	else
	{
		if (cchannel->check_mode('t') && cchannel->getOperByNick(this->userN.getNickN()) == false)
			return (sendMessage(RED "Error: You need to be channel operator to set the topic!" RESET, "481"));
		else
		{
			channelTopic = this->_args[1];
			cchannel->setTopic(channelTopic);
		}
	}
}