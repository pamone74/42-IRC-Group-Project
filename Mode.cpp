/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamone <pamone@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 23:51:20 by pamone            #+#    #+#             */
/*   Updated: 2024/11/23 23:51:21 by pamone           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Command.hpp"

void Command::Mode_cmd() 
{
    Channel *channel;
    Client *user;

    if (this->_args.size() < 1)
        return sendMessage(RED "Error: usage: /MODE <channel>  [[+|-]modechars [parameters]]" RESET, "461");
    
    if (this->_args[0][0] != '#')
        this->_args[0] = '#' + this->_args[0];
    
    channel = this->_ircServer.getChannelByName(this->_args[0]);
    if (channel == NULL)
        return sendMessage(RED "Error: Channel Not Found!" RESET , "403", 0, this->_args[0]);
    
    if (this->_args.size() == 1)
        return sendMessage(RED "Error: The channel mode is +" RESET + channel->getMode(), "1");

    if (channel->getOperByNick(this->userN.getNickN()) == false)
        return sendMessage(RED "Error: You need be operator to use Mode cmd!" RESET, "482", 0, channel->getChannelName());

    size_t arg = 1;
    while (arg < this->_args.size()) 
	{
        int next = 1;
        if (this->_args[arg][0] == '-') { // Remove mode
            for (unsigned long i = 1; i < this->_args[arg].size(); i++) 
			{
                if (this->_args[arg][i] == 'o') 
				{
                    while (arg + next < this->_args.size() && this->_args[arg + next][0] != '+' && this->_args[arg + next][0] != '-')
					 {
                        user = channel->getOperByNick(this->_args[arg + next]);
                        if (user == NULL) 
						{
                            next++;
                            continue;
                        }
                        channel->removeOper(user);
                        next++;
                    }
                } 
				else 
				{
                    size_t found = channel->getMode().find(this->_args[arg][i]);
                    if (found != std::string::npos) 
					{
                        std::string mode = channel->getMode().erase(found, 1);
                        channel->setMode(mode);
                    }
                }
            }
        } 
		else if (this->_args[arg][0] == '+')
		{ // Add mode
            for (size_t i = 1; i < this->_args[arg].size(); i++) 
			{
                if (!(this->is_valid_mode(this->_args[arg][i])))
                    return sendMessage(RED "Error: NOT a valid mode" RESET, "472");

                if (channel->getMode().find(this->_args[arg][i]) == std::string::npos && this->_args[arg][i] != 'o') 
				{
                    std::string mode = channel->getMode() + this->_args[arg][i];
                    channel->setMode(mode);
                }

                switch (this->_args[arg][i]) 
				{
                    case 'o': 
					{
                        while (arg + next < this->_args.size() && this->_args[arg + next][0] != '+' && this->_args[arg + next][0] != '-') 
						{
                            user = channel->getClientByNick(this->_args[arg + next]);
                            if (user != NULL) 
							{
                                if (channel->getOperByNick(this->_args[arg + next]) == NULL) 
								{
                                    channel->addOper(user);
                                }
                            } 
							else
                                sendMessage(RED "Error: User needs to be a member of the channel to be an Operator!" RESET, "442");
                            next++;
                        }
                        break;
                    }
                    case 'k':
                        if (arg + next >= this->_args.size()) 
						{
                            size_t found = channel->getMode().find('k');
                            if (found != std::string::npos) 
							{
                                std::string mode = channel->getMode().erase(found, 1);
                                channel->setMode(mode);
                            }
                            return sendMessage(RED "Error: missing parameter" RESET, "461");
                        }
                        channel->setPassword(this->_args[arg + next]);
                        next++;
                        break;
                    case 'l': 
					{
                        if (arg + next >= this->_args.size()) 
						{
                            size_t found = channel->getMode().find('l');
                            if (found != std::string::npos) 
							{
                                std::string mode = channel->getMode().erase(found, 1);
                                channel->setMode(mode);
                            }
                            return sendMessage(RED "Error: missing parameter" RESET, "461");
                        }
                        long long limit = atoll(this->_args[arg + next].c_str());
                        if (limit < 1) 
						{
                            size_t found = channel->getMode().find('l');
                            if (found != std::string::npos) 
							{
                                std::string mode = channel->getMode().erase(found, 1);
                                channel->setMode(mode);
                            }
                            return sendMessage(RED "Error: invalid limit" RESET, "461");
                        }
                        channel->setLimit(strtoul(this->_args[arg + next].c_str(), 0, 0));
                        next++;
                        break;
                    }
                    default:
                        break;
                }
            }
        } 
		else 
		{
            return sendMessage(RED "Error: NOT a valid mode" RESET, "472");
        }
        arg += next;
    }
}
